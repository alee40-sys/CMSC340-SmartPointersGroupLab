Group Lab: Mastering Modern C++ Smart Pointers
Objective
Transition from manual memory management (new/delete) to RAII-based resource management using std::unique_ptr, std::shared_ptr, and std::weak_ptr. By the end of this lab, you will be able to express ownership semantics in code, prevent memory leaks, and break circular references.
Starter Code: Copy the base logging class below into main.cpp. This class tracks object creation and destruction so you can verify zero memory leaks without external tools (though running with Valgrind or -fsanitize=address is encouraged).
C++
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

Part 1: Exclusive Ownership (std::unique_ptr)
std::unique_ptr models sole ownership. It cannot be copied, only moved, ensuring that only one pointer is responsible for deleting the resource.
Task 1.1: Creation and Transfer
In main(), write a function createResource(const std::string& name) that returns a std::unique_ptr<Resource> using std::make_unique.
Create a unique_ptr named res1 for "DatabaseConnection" and call res1->use().
Attempt to copy res1 into a new variable res2 (auto res2 = res1;). Observe the compiler error, then comment it out.
Transfer ownership from res1 to res2 using std::move.
Verify that res1 is now nullptr before calling res2->use().
Task 1.2: Storing in Containers
Create a std::vector<std::unique_ptr<Resource>> called pool.
Add "Socket_A" and "Socket_B" to pool using std::make_unique and push_back (or emplace_back).
Move res2 into pool.
Iterate over pool using a range-based for loop (const auto& item : pool) and call item->use(). Question: Why must item be a reference (const auto&) rather than a value (auto)?

Part 2: Shared Ownership (std::shared_ptr)
std::shared_ptr models shared ownership via reference counting. The underlying object is destroyed only when the last shared_ptr pointing to it goes out of scope.
Task 2.1: Reference Counting
Create a std::shared_ptr<Resource> named shared1 for "ConfigFile" using std::make_shared.
Print shared1.use_count().
Create an inner scope { ... }. Inside it, create shared2 = shared1 and a std::vector<std::shared_ptr<Resource>> containing a copy of shared1.
Print shared1.use_count() inside the inner scope.
Exit the inner scope and print shared1.use_count() again. Observe when the destructor message prints.

Part 3: Breaking Circular References (std::weak_ptr)
When two objects hold std::shared_ptr references to each other, their reference counts never reach zero, causing a memory leak. std::weak_ptr observes an object without contributing to its reference count.
Task 3.1: Reproduce the Leak Add the following Node struct to your file and instantiate two nodes (nodeA and nodeB) in a local scope, linking them to each other:
C++
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
Run the program and notice that the [-] Node destroyed messages never appear.
Task 3.2: Fix the Cycle
Change partner in struct Node from std::shared_ptr<Node> to std::weak_ptr<Node>.
Add a method void pingPartner() const to Node. Inside it, use partner.lock() to safely obtain a temporary std::shared_ptr<Node>. If valid, print "Pinged " + lockedPartner->id; otherwise, print "Partner no longer exists".
Test pingPartner() both while nodeB is alive and after nodeB has gone out of scope (e.g., via nodeB.reset()).
