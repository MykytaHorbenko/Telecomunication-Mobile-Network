#include "NetConfAgent.hpp"

using namespace std;

bool NetConfAgent::initSysrepo() 
{
    Connection = std::make_shared<sysrepo::Connection>();
    Session = std::make_shared<sysrepo::Session>(Connection);
    std::cout<<"Session began"<<std::endl;
    return 1;
}
/* Helper function for printing nodes. */
static void print_node(libyang::S_Data_Node &node)
{
    libyang::S_Schema_Node schema = node->schema();

    //cout << nodetype2str(schema->nodetype()) << " \"" << schema->name() << '\"' << endl;
    cout << '\t' << "Path: " << node->path() << endl;
    //cout << '\t' << "Default: " << (node->dflt() ? "yes" : "no") << endl;

    /* type-specific print */
    switch (schema->nodetype()) {
    case LYS_CONTAINER:
    {
        // libyang::Schema_Node_Container scont(schema);

        // cout << '\t' << "Presence: " << (scont.presence() ? "yes" : "no") << endl;
        break;
    }
    case LYS_LEAF:
    {
        libyang::Data_Node_Leaf_List leaf(node);
        libyang::Schema_Node_Leaf sleaf(schema);

        cout << '\t' << "Value: \"" << leaf.value_str() << '\"' << endl;
        //cout << '\t' << "Is key: " << (sleaf.is_key() ? "yes" : "no") << endl;
        break;
    }
    case LYS_LEAFLIST:
    {
        libyang::Data_Node_Leaf_List leaflist(node);

        cout << '\t' << "Value: \"" << leaflist.value_str() << '\"' << endl;
        break;
    }
    case LYS_LIST:
    {
        // libyang::Schema_Node_List slist(schema);

        // cout << '\t' << "Keys:";
        // for (libyang::S_Schema_Node_Leaf &key : slist.keys()) {
        //     cout << ' ' << key->name();
        // }
        // cout << endl;
        break;
    }
    default:
        break;
    }

    cout << endl;
}

bool NetConfAgent::fetchData()
{
    //sr_datastore_t ds = SR_DS_RUNNING;
    const char *xpath = "/mobile-network:core/subscribers[number='001']";

    try {
        libyang::S_Data_Node data = Session->get_data(xpath);
        /* go through all top-level siblings */
        for (libyang::S_Data_Node &root : data->tree_for()) {
            /* go through all the children of a top-level sibling */
            for (libyang::S_Data_Node &node : root->tree_dfs()) {
                print_node(node);
            }
        }
    } catch( const std::exception& e ) {
        cout << e.what() << endl;
    }
    return 1;
}

/* Helper function for printing events. */
const char *ev_to_str(sr_event_t ev) {
    switch (ev) {
    case SR_EV_CHANGE:
        return "change";
    case SR_EV_DONE:
        return "done";
    case SR_EV_ABORT:
    default:
        return "abort";
    }
}

/* Helper function for printing changes given operation, old and new value. */
void print_change(sysrepo::S_Change change) 
{
    cout << endl;
    switch(change->oper()) {
    case SR_OP_CREATED:
        if (nullptr != change->new_val()) {
           cout <<"CREATED: ";
           cout << change->new_val()->to_string();
        }
        break;
    case SR_OP_DELETED:
        if (nullptr != change->old_val()) {
           cout << "DELETED: ";
           cout << change->old_val()->to_string();
        }
    break;
    case SR_OP_MODIFIED:
        if (nullptr != change->old_val() && nullptr != change->new_val()) {
           cout << "MODIFIED: ";
           cout << "old value ";
           cout << change->old_val()->to_string();
           cout << "new value ";
           cout << change->new_val()->to_string();
        }
    break;
    case SR_OP_MOVED:
        if (nullptr != change->old_val() && nullptr != change->new_val()) {
           cout << "MOVED: ";
           cout << change->new_val()->xpath();
           cout << " after ";
           cout << change->old_val()->xpath();
        } else if (nullptr != change->new_val()) {
           cout << "MOVED: ";
           cout << change->new_val()->xpath();
           cout << " first";
        }
    break;
    }
}

/* Helper function for exiting app. */
volatile int exit_application = 0;
static void sigint_handler(int signum)
{
    exit_application = 1;
}

bool NetConfAgent::subscribeForModelChanges()

{
    cout << "To exit from app press ctrl-c" << endl;
    #define MAX_LEN 100

    const char *module_name = "mobile-network";
    try {
        /* subscribe for changes in running config */
        Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
        auto cb = [] (sysrepo::S_Session Session, const char *module_name, const char *xpath, sr_event_t event,
            uint32_t request_id) {
            char change_path[MAX_LEN];

            try {
                cout << "\n\n ========== Notification " << ev_to_str(event) << " =============================================";

                cout << "\n\n ========== CHANGES: =============================================\n" << endl;

                snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);

                auto it = Session->get_changes_iter(change_path);

                while (auto change = Session->get_change_next(it)) {
                    print_change(change);
                }

                cout << "\n\n ========== END OF CHANGES =======================================\n" << endl;

            } catch( const std::exception& e ) {
                cout << e.what() << endl;
            }
            return SR_ERR_OK;

        };
        Subscribe->module_change_subscribe(module_name, cb);

        /* loop until ctrl-c is pressed / SIGINT is received */
        signal(SIGINT, sigint_handler);
        while (!exit_application) {
            sleep(1000);  /* or do some more useful work... */
        }

    } catch( const std::exception& e ) {
        cout << e.what() << endl;
        //return -1;
    }
    return 1;
}

bool NetConfAgent::registerOperData()
{
    #define MAX_LEN 100

    const char *module_name = "mobile-network";
    try {
        cout << "Application will provide data of " << module_name << endl;

        auto Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
        auto cb2 = [] (sysrepo::S_Session Session, const char *module_name, const char *path, const char *request_xpath,
            uint32_t request_id, libyang::S_Data_Node &parent) {
                cout << "Lambda start" << endl;
            cout << "\n\n ========== CALLBACK CALLED TO PROVIDE \"" << path << "\" DATA ==========\n" << endl;
            libyang::S_Context ctx = Session->get_context();
            libyang::S_Module mod = ctx->get_module(module_name);

            //parent->new_path(ctx, "/mobile-network:core/subscribers[number='001']/userName", "Bob", LYD_ANYDATA_CONSTSTRING, 0);

            auto sub = std::make_shared<libyang::Data_Node>(parent, mod, "subscribers");
            auto number = std::make_shared<libyang::Data_Node>(sub, mod, "number", "002");
            auto user_name = std::make_shared<libyang::Data_Node>(sub, mod, "userName", "Bob");

            return SR_ERR_OK;
            
            cout << "Lambda finish" << endl;
        };
        Subscribe->oper_get_items_subscribe(module_name, cb2, "/mobile-network:core/subscribers[number='001']");
        
        cout << "OperData changed" << endl;

        /* loop until ctrl-c is pressed / SIGINT is received */
        signal(SIGINT, sigint_handler);
        while (!exit_application) {
            sleep(1000);  /* or do some more useful work... */
        }

        cout << "Application exit requested, exiting." << endl;

    } catch( const std::exception& e ) {
        cout << e.what() << endl;
        return -1;
    }
}

bool NetConfAgent::changeData()
{
    //Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
    try {

        const char *xpath = "/mobile-network:core/subscribers[number='001']/incomingNumber";
        const char *newValue = "5555555";
        auto value = std::make_shared<sysrepo::Val>(newValue);
        Session->set_item(xpath, value);

        Session->apply_changes();
    } catch( const std::exception& e ) {
        cout << e.what() << endl;
    }
    cout << "Data changed" << endl;
}

void print_value(sysrepo::S_Val value)
{
    cout << value->xpath();
    cout << " ";
    switch (value->type()) {
    case SR_CONTAINER_T:
    case SR_CONTAINER_PRESENCE_T:
        cout << "(container)" << endl;
        break;
    case SR_LIST_T:
        cout << "(list instance)" << endl;
        break;
    case SR_STRING_T:
        cout << "= " << value->data()->get_string() << endl;;
        break;
    case SR_BOOL_T:
    if (value->data()->get_bool())
            cout << "= true" << endl;
    else
            cout << "= false" << endl;
        break;
    case SR_ENUM_T:
        cout << "= " << value->data()->get_enum() << endl;;
        break;
    case SR_UINT8_T:
        cout << "= " << unsigned(value->data()->get_uint8()) << endl;
        break;
    case SR_UINT16_T:
        cout << "= " << unsigned(value->data()->get_uint16()) << endl;
        break;
    case SR_UINT32_T:
        cout << "= " << unsigned(value->data()->get_uint32()) << endl;
        break;
    case SR_UINT64_T:
        cout << "= " << unsigned(value->data()->get_uint64()) << endl;
        break;
    case SR_INT8_T:
        cout << "= " << value->data()->get_int8() << endl;
        break;
    case SR_INT16_T:
        cout << "= " << value->data()->get_int16() << endl;
        break;
    case SR_INT32_T:
        cout << "= " << value->data()->get_int32() << endl;
        break;
    case SR_INT64_T:
        cout << "= " << value->data()->get_int64() << endl;
        break;
     case SR_IDENTITYREF_T:
        cout << "= " << value->data()->get_identityref() << endl;
        break;
    case SR_BITS_T:
        cout << "= " << value->data()->get_bits() << endl;
        break;
    case SR_BINARY_T:
        cout << "= " << value->data()->get_binary() << endl;
        break;
    default:
        cout << "(unprintable)" << endl;
    }
    return;
}

bool NetConfAgent::subscribeForRpc()
{
    const char *module_name = "mobile-network";
    try {

        printf("Application will make an rpc call in %s\n", module_name);

        /* subscribe for changes in running config */
        auto Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
        auto cbVals = [](sysrepo::S_Session session, const char* op_path, const sysrepo::S_Vals input, sr_event_t event, 
        uint32_t request_id, sysrepo::S_Vals_Holder output) 
        {
            cout << "\n ========== RPC CALLED ==========\n" << endl;

            auto out_vals = output->allocate(1);

            //for(size_t n = 0; n < input->val_cnt(); ++n)
                print_value(input->val(0));

            out_vals->val(0)->set("/mobile-network:change-input-number/status",
                    "inputNumber changed",
                    SR_STRING_T);

            return SR_ERR_OK;
        };

        auto cbTree = [] (sysrepo::S_Session session, const char *op_path, const libyang::S_Data_Node input, sr_event_t event,
                uint32_t request_id, libyang::S_Data_Node output) {
            cout << "\n ========== RPC TREE CALLED ==========\n" << endl;
            cout << input->print_mem(LYD_XML, LYP_FORMAT);

            libyang::S_Context ctx = session->get_context();

            output->new_path(ctx, "status", "inputNumber changed", LYD_ANYDATA_CONSTSTRING, LYD_PATH_OPT_OUTPUT);

            return SR_ERR_OK;
        };

        cout << "\n ========== SUBSCRIBE TO RPC CALL ==========\n" << endl;

        Subscribe->rpc_subscribe("/mobile-network:change-input-number", cbVals, 1);

        auto in_vals = std::make_shared<sysrepo::Vals>(1);

        in_vals->val(0)->set("/mobile-network:change-input-number/input-number",
                           "00000", SR_STRING_T);

        cout << "\n ========== START RPC CALL ==========\n" << endl;
        auto out_vals = Session->rpc_send("/mobile-network:change-input-number", in_vals);

        cout << "\n ========== PRINT RETURN VALUE ==========\n" << endl;
        for(size_t n=0; n < out_vals->val_cnt(); ++n)
            print_value(out_vals->val(n));

        cout << "\n ========== SUBSCRIBE TO RPC TREE CALL ==========\n" << endl;
        Subscribe->rpc_subscribe_tree("/mobile-network:change-input-number", cbTree, 0, SR_SUBSCR_CTX_REUSE);

        libyang::S_Context ctx = Connection->get_context();
        libyang::S_Module mod = ctx->get_module(module_name);
        auto in_trees = std::make_shared<libyang::Data_Node>(ctx, "/mobile-network:change-input-number", nullptr, LYD_ANYDATA_CONSTSTRING, 0);
        std::make_shared<libyang::Data_Node>(libyang::Data_Node(in_trees, mod, "input-number", "00000"));

        cout << "\n ========== START RPC TREE CALL ==========\n" << endl;
        auto out_trees = Session->rpc_send(in_trees);

        cout << "\n ========== PRINT RETURN VALUE ==========\n" << endl;
        cout << out_trees->print_mem(LYD_XML, LYP_FORMAT);

        cout << "\n ========== END PROGRAM ==========\n" << endl;
    } catch( const std::exception& e ) {
        cout << e.what() << endl;
        return -1;
    }
}