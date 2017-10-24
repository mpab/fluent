#include <string>
#include "node.h"

#define exec context::execute_block
#define addi context::add_instruction

namespace context {
    using namespace std;
    using namespace node;

    Node* add_instruction(int opcode, int count, Node* n1, Node* n2 = nullptr, Node* n3 = nullptr);

    Node* get_symbolic_node(string symbol_name);
    void free_unused_nodes();
    void free_all_nodes();

    void execute_blocks();
    void execute_block(Node *n);

    const Node* active_block();
    const Node* last_created_node();

    enum ExecuteMode {IMMEDIATE, BUFFERED};
    void set_execute_mode(ExecuteMode mode);

    void inspect();
}
