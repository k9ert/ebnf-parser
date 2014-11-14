struct node {
        char type;
        union {
      char tsym;
      struct header* nsym;
      } head;
        struct node* suc;
        struct node* alt;
};

struct header {
        char sym;
        struct node* entry;
        struct header* suc;
};

struct litlist {
        char lit[30];
        char sym;
        struct litlist* next;
};

struct freelist {
        void* pointer;
        struct freelist* next;
};

