#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Tree.h"
#include "TreeDot.h"

static FILE* LogFile = 0; 
static FILE* DotFile = 0;
static FILE* BaseFile = 0;
static FILE* Base = 0;

static void EndProgramm() {
	fprintf(LogFile,  "<font color=\"green\">List log file successfully closed</font>\n");
	fclose(LogFile);	
}

TreeErrors TreeCtor(Tree* tree) {
	CHECKCONDITION(tree == nullptr, NULL_PTR, "Null ptr on Tree");

	#ifndef RELEASE
	LogFile = fopen(NameLogFile, "w");
	CHECKCONDITION(LogFile == nullptr, CANT_CREATE_LOGS, "Can not create log file");
	atexit(EndProgramm);	
	#endif

	tree->is_ctor = true;
	tree->is_dtor = false;
	tree->size = 0;
	tree->root = nullptr;
	
	if (TreeOk) return TREE_ERROR; 

	return OK;
}

TreeErrors TreeDtor(Tree* tree) {
	CHECKCONDITION(tree == nullptr, NULL_PTR, "Null ptr on Tree");

	if (TreeOk) return TREE_ERROR; 

	tree->is_dtor = true;
	DestroyTree(tree->root);

	return OK;
}

TreeErrors TreeInsert(Node* node, const char* const str) {
	CHECKCONDITION(str == nullptr, NULL_PTR, "Null ptr on string");
	CHECKCONDITION(node == nullptr, NULL_PTR, "Null ptr on node");
	size_t size_str = strlen(str);

	char* copy_str = (char*) calloc(size_str, sizeof(char));
	CHECKCONDITION(copy_str == nullptr, NULL_PTR, "Null ptr on copy str");
	
	memmove(copy_str, str, size_str);
	node->value = copy_str;

	return OK;
}

void DestroyTree(Node* root) {
	if (root == nullptr) return;

	DestroyTree(root->left);
	DestroyTree(root->right);
	free(root->value);
	free(root);

	return;
}

Node* NewNode(const char* const str) {
	Node* node = (Node*) calloc(1, sizeof(Node));
	
	if (TreeInsert(node, str) == NULL_PTR) 
		return nullptr;

	return node;
}

TreeErrors TreeGraph(Tree* tree) {
	CHECKCONDITION(tree == nullptr, NULL_PTR, "Null ptr on tree");

	DotFile = fopen(NameDotFile, "w");
	fprintf(DotFile, "digraph G {\n");
	NUMBER_DEPTH(tree->size);

	DrawNode(tree->root, 1);

	fprintf(DotFile, "}");
	fclose(DotFile);
	DotFile = nullptr;

	const char command[] = "dot -T png TreeDot.dot -o Tree.png";
	
	system(command);

	return OK;
}


void DrawNode(Node* node, const size_t depth) {
	
	DRAW_NODE(node->value, depth);
	if (node->left != nullptr) {
		DRAW_CONNECT(node->value, (node->left)->value);
		DrawNode(node->left, depth + 1);
	}
	if (node->right != nullptr) {
		DRAW_CONNECT(node->value, (node->right)->value);
		DrawNode(node->right, depth + 1);
	}

	return;
}

TreeErrors PrintPreOrder(Tree* tree) {
	CHECKCONDITION(tree == nullptr, NULL_PTR, "Null ptr on tree");

	BaseFile = fopen(NameBaseFile, "w");

	PrintNode(tree->root);

	fclose(BaseFile);

	return OK;
}

void PrintNode(Node* node) {
	if (!node) {
		fprintf(BaseFile, "nil");
		return;
	}

	fprintf(BaseFile, "(%s", node->value);
	PrintNode(node->left);
	PrintNode(node->right);
	fprintf(BaseFile, ")");
}

Tree CreateTree(const char* file) {

	Tree tree;
	TreeCtor(&tree);

	char* buffer = CreateBuffer(file);

	size_t total_pos = 0;

	tree.root = ReadNode(&tree, buffer, &total_pos);

	free(buffer);

	return tree;
}

Node* ReadNode(Tree* tree, const char* buffer, size_t* position) {
	if (buffer[*position] == '\0') return nullptr;

	Node* node;

	if (buffer[*position] == '(') {
		(*position)++;

		size_t begin = *position;
		
		while (buffer[*position] != '(' && buffer[*position] != 'n') {
			(*position)++;
		}

		char* str = (char*) calloc(*position - begin, sizeof(char));
		memcpy(str, buffer + begin, *position - begin);
		
		node = NewNode(str);

		tree->size++;
		
		if (buffer[*position] != 'n') {
			node->left = ReadNode(tree, buffer, position);
		} else {
			(*position) += 3;
			node->left = nullptr;
		}

		if (buffer[*position] != 'n') {
			node->right = ReadNode(tree, buffer, position);
		} else {
			(*position) += 3;
			node->right = nullptr;
		}
	}

	if (buffer[*position] == ')') {
		(*position)++;
	}

	return node;
}

char* CreateBuffer(const char* file) {
	FILE* Base = fopen(file, "r");

	struct stat buff = {};
    fstat (fileno (Base), &buff);

    char* buf = (char*) calloc(buff.st_size+1, sizeof(char));

    fread(buf, sizeof(char), buff.st_size, Base);

    buf[buff.st_size] = '\0';

	fclose(Base);

	return buf;
}










