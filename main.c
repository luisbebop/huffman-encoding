#include <stdio.h>
#include <string.h>
 
typedef struct node_t {
	struct node_t *left, *right;
	int freq;
	char c;
} *node;
FILE *f;
 
struct node_t pool[256] = {{0}};
node qqq[255], *q = qqq - 1;
int n_nodes = 0, qend = 1;
char *code[128] = {0}, buf[1024];
 
node new_node(int freq, char c, node a, node b)
{
	node n = pool + n_nodes++;
	if (freq) n->c = c, n->freq = freq;
	else {
		n->left = a, n->right = b;
		n->freq = a->freq + b->freq;
	}
	return n;
}
 
/* priority queue */
void qinsert(node n)
{
	int j, i = qend++;
	while ((j = i / 2)) {
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}
 
node qremove()
{
	int i, l;
	node n = q[i = 1];
 
	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}
 
/* walk the tree and put 0s and 1s */
void build_code(node n, char *s, int len)
{
	static char *out = buf;
	if (n->c) {
		s[len] = 0;
		strcpy(out, s);
		code[n->c] = out;
		out += len + 1;
		return;
	}
 
	s[len] = '0'; build_code(n->left,  s, len + 1);
	s[len] = '1'; build_code(n->right, s, len + 1);
}
 
void init(const char *s)
{
	int i, freq[128] = {0};
	char c[16];
 
	while (*s) freq[(int)*s++]++;
 
	for (i = 0; i < 128; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));
 
	while (qend > 2) 
		qinsert(new_node(0, 0, qremove(), qremove()));
 
	build_code(q[1], c, 0);
}
 
void encode(const char *s, char *out)
{
	while (*s) {
		strcpy(out, code[*s]);
		out += strlen(code[*s++]);
	}
}
 
void decode(const char *s, node t)
{
	node n = t;
	while (*s) {
		if (*s++ == '0') n = n->left;
		else n = n->right;
 
		if (n->c) putchar(n->c), n = t;
	}
 
	putchar('\n');
	if (t != n) printf("garbage input\n");
}

void saveTree(node n)
{
	
	f = fopen("huffman.gv", "a+");
	if(n->c)
		fprintf(f,"c%c [label=\"{{%c|%d}|%s}\", color=forestgreen, shape=record];\n", n->c, n->c, n->freq, code[n->c]);
	else
		fprintf(f,"a%p [label=\"%d\", color=blue];\n", n, n->freq);

	if(n->right) {
		fprintf(f,"a%p", n);
		if(n->right->c)
			fprintf(f," -> c%c [label=1];\n", n->right->c);
		else
			fprintf(f," -> a%p [label=1];\n", n->right);

	 	saveTree(n->right);
	}

	if(n->left) {
		fprintf(f,"a%p", n);
		if(n->left->c)
			fprintf(f," -> c%c [label=0];\n", n->left->c);
		else
			fprintf(f," -> a%p [label=0];\n", n->left);

	 	saveTree(n->left);
	}
}
 
int main(int argc, char *argv[])
{
	int i;
	char buf[1024];
	const char *str = NULL;

	// Use command line argument if suplied
	if(argc == 2) {
		str = argv[1];
	}
	else {
		str = "this is an example for huffman encoding";
	}
	
	init(str);
	for (i = 0; i < 128; i++)
		if (code[i]) printf("'%c': %s\n", i, code[i]);
 
	encode(str, buf);
	printf("encoded: %s\n", buf);
 
	printf("decoded: ");
	decode(buf, q[1]);

	f = fopen("huffman.gv", "w");
	fclose(f);

	f = fopen("huffman.gv", "a+");
	fprintf(f, "Digraph G {\n");
	fseek(f,-1,SEEK_CUR);
	fprintf(f, "}\n");
	saveTree(q[1]);
	fclose(f);
 
	return 0;
}