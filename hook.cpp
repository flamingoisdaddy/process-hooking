#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <fstream>
#include <random>

const std::string reset = "\033[0m";
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string blue = "\033[34m";
const std::string magenta = "\033[35m";
const std::string cyan = "\033[36m";
const std::string white = "\033[37m";

std::vector<pid_t> getrandompid(int count) {
    std::vector<pid_t> pids;
    DIR* dir = opendir("/proc");
    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            pid_t pid = atoi(entry->d_name);
            pids.push_back(pid);
        }
    }
    closedir(dir);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::shuffle(pids.begin(), pids.end(), std::default_random_engine(std::rand()));

    if (pids.size() > count) {
        pids.resize(count);
    }

    return pids;
}

long getmemaddr(pid_t pid) {
    std::ostringstream path;
    path << "/proc/" << pid << "/maps";
    std::ifstream mapsFile(path.str());
    std::string line;

    if (std::getline(mapsFile, line)) {
        std::istringstream iss(line);
        std::string addressRange;
        iss >> addressRange;
        std::string startAddress = addressRange.substr(0, addressRange.find('-'));
        return std::stol(startAddress, nullptr, 16);
    }
    return -1;
}

void procattach(pid_t pid) {
    if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) == -1) {
        std::cerr << red << "ptrace attach failed" << reset << std::endl;
        return;
    }

    waitpid(pid, nullptr, 0);
    std::cout << green << "[+] attached to process " << pid << reset << std::endl;

    long address = getmemaddr(pid);
    if (address == -1) {
        std::cerr << red << "[!] cannot get mem addr" << reset << std::endl;
        ptrace(PTRACE_DETACH, pid, nullptr, nullptr);
        return;
    }

    errno = 0;
    long data = ptrace(PTRACE_PEEKDATA, pid, (void*)address, nullptr);
    if (data == -1 && errno) {
        std::cerr << red << "ptrace peekdata failed" << reset << std::endl;
    } else {
        std::cout << blue << "[+] data at addr " << std::hex << address << ": " << std::hex << data << std::dec << reset << std::endl;
    }

    if (ptrace(PTRACE_DETACH, pid, nullptr, nullptr) == -1) {
        std::cerr << red << "ptrace detach failed" << reset << std::endl;
    } else {
        std::cout << green << "[+] detached from process " << pid << reset << std::endl;
    }
}

int main() {
    const int numPIDs = 3;
    auto pids = getrandompid(numPIDs);

    if (pids.empty()) {
        std::cerr << red << "[+] not found" << reset << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << yellow << "[+] random selected pids:" << reset << std::endl;
    for (size_t i = 0; i < pids.size(); ++i) {
        std::cout << i + 1 << ": " << pids[i] << std::endl;
    }

    int choice;
    std::cout << cyan << "[+] select pid (1-" << numPIDs << "): " << reset;
    std::cin >> choice;

    if (choice < 1 || choice > numPIDs) {
        std::cerr << red << "[!] invalid" << reset << std::endl;
        return EXIT_FAILURE;
    }

    pid_t selectedPID = pids[choice - 1];
    procattach(selectedPID);

    return EXIT_SUCCESS;
}
