#ifndef __TREE_H__
#define __TREE_H__
#include <cstddef>

#ifndef RELEASE
	#define CHECKCONDITION(Condition, ErrorCode, ErrorMessage) do {              \
                                                            if (Condition)  { \
                                                                if (LogFile == NULL) {\
                                                                    LogFile = fopen(NameLogFile, "w"); \
																	atexit(EndProgramm); }     \
                                                                fprintf(LogFile,"<font color=\"red\">ERROR:</font> %s in %s(%d) %s" "\n",  \
																	ErrorMessage, __FILE__, __LINE__,__PRETTY_FUNCTION__); \
                                                                return ErrorCode; \
                                                            }    }  while(0) 															
#else	
	#define CHECKCONDITION(Condition, ErrorCode, ErrorMessage) do {              \
                                                            if (Condition)  { \
                                                                return ErrorCode; \
                                                            }    }  while(0)
#endif


const char NameLogFile[] = "TreeLogs.htm";
const char NameDotFile[] = "TreeDot.dot";
const char NameBaseFile[] = "TreeBase.txt";

const int MAX_LENGTH_SENT = 100;

struct Node {
	char* value;
	Node* left = nullptr;
	Node* right = nullptr;
};

struct Tree {
	size_t size;
	Node* root;
	bool is_ctor;
	bool is_dtor;
};

enum TreeErrors{
	OK = 0,
	NO_MEMORY = 1,
	NULL_PTR = 2,
	CANT_CREATE_LOGS = 3,
	TREE_ERROR = -1
};

TreeErrors TreeCtor(Tree* tree);

TreeErrors TreeDtor(Tree* tree);
TreeErrors TreeOk (Tree* tree);
TreeErrors TreeDump(Tree* tree);

void DestroyTree(Node* root);


bool TreeFind(const Tree* tree, const char* const str);

TreeErrors TreeInsert(Node* node, const char* const str); 

Node* NewNode(const char* const str);

TreeErrors TreeGraph(Tree* tree);
void DrawNode(Node* node, const size_t depth);

TreeErrors PrintPreOrder(Tree* tree);
void PrintNode(Node* node);

Tree CreateTree(const char* file);
Node* ReadNode(Tree* tree, const char* buffer, size_t* position);
char* CreateBuffer(const char* file);

#endif