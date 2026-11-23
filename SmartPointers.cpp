#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Resource {
public:
    explicit Resource(std::string name) : name_(std::move(name)) {
        std::cout << "[+] Acquired Resource: " << name_ << "\n";
    }
    ~Resource() {
        std::cout << "[-] Released Resource: " << name_ << "\n";
    }
    void use() const {
        std::cout << "[*] Using Resource: " << name_ << "\n";
    }
    const std::string& name() const { return name_; }

private:
    std::string name_;
};

struct Node {
    std::string id;
    std::shared_ptr<Node> partner; // BUG: Causes a cycle!

    explicit Node(std::string i) : id(std::move(i)) {
        std::cout << "[+] Node created: " << id << "\n";
    }
    ~Node() {
        std::cout << "[-] Node destroyed: " << id << "\n";
    }
};

int main() {
}