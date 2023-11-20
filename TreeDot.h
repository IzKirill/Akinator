#ifndef __TREEDOT_H__
#define __TREEDOT_H__

#define NUMBER_DEPTH(depth) do {                                                                     \
								fprintf(DotFile, "{node[shape=plaintext];\n edge[color=white];\n");  \
								for(size_t i = 1; i <= depth; i++) {                                  \
									if (i != 1) {                                                       \
										fprintf(DotFile, " -> \"%lu\"", i);}                             \
									else {                                                             \
										fprintf(DotFile, "\"1\"");}                                      \
								}                                                                      \
								fprintf(DotFile,";}\n");                                                 \
							} while(0);                                                                 

#define DRAW_NODE(string, depth) do {                                 \
								  	fprintf(DotFile,"{rank = same;");        \
									fprintf(DotFile,"\"%s\";", string);      \
									fprintf(DotFile,"\"%lu\";}\n", depth);    \
								  } while(0);
#define DRAW_CONNECT(from, to) do {                                    \
									fprintf(DotFile,"\"%s\" -> \"%s\"\n", from, to);  \
								} while(0);                                   
#endif