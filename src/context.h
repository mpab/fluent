#include <string>
#include "node.h"

#define ex context::execute
#define opr context::func

namespace context {
    using namespace std;
    using namespace node;

    Node* func(int opcode, int count, Node* n1, Node* n2 = nullptr, Node* n3 = nullptr);

    Node* get_node(string k, bool warn_if_not_defined = true);
    void free_unused_nodes();
    void free_all_nodes();

    void execute(Node *n);
    void execute();
    Node* active_frame();

    enum ExecuteMode {IMMEDIATE, BUFFERED};
    void set_execute_mode(ExecuteMode mode);
}
