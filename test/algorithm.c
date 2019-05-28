#include <rbtree_augmented.h>
#include<time.h>
#include<stdlib.h>
#include<kfifo.h>
#include <string.h>




/**************************test for rbtree begin****************/
#define NODES       100
#define PERF_LOOPS  100000
#define CHECK_LOOPS 100

struct test_node {
	struct rb_node rb;
	unsigned int key;

	/* following fields used for testing augmented rbtree functionality */
	unsigned int val;
	unsigned int augmented;
};



static struct rb_root root={NULL};
static struct test_node nodes[NODES];





static void init(void)
{
	int i;
	srand(time(NULL));
	for (i = 0; i < NODES; i++) {
		nodes[i].key = rand();
		nodes[i].val = rand();
	}
}
static void insert(struct test_node *node, struct rb_root *root)
{
	struct rb_node **new = &root->rb_node, *parent = NULL;
	unsigned int key = node->key;

	while (*new) {
		parent = *new;
		if (key < rb_entry(parent, struct test_node, rb)->key)
			new = &parent->rb_left;
		else
			new = &parent->rb_right;
	}

	rb_link_node(&node->rb, parent, new);
	rb_insert_color(&node->rb, root);
}

static inline void erase(struct test_node *node, struct rb_root *root)
{
	rb_erase(&node->rb, root);
}



/********************************augment_callbacks************************************/
static inline unsigned int augment_recompute(struct test_node *node)
{
	unsigned int max = node->val, child_augmented;
	if (node->rb.rb_left) {
		child_augmented = rb_entry(node->rb.rb_left, struct test_node,
					   rb)->augmented;
		if (max < child_augmented)
			max = child_augmented;
	}
	if (node->rb.rb_right) {
		child_augmented = rb_entry(node->rb.rb_right, struct test_node,
					   rb)->augmented;
		if (max < child_augmented)
			max = child_augmented;
	}
	return max;
}



RB_DECLARE_CALLBACKS(static, augment_callbacks, struct test_node, rb,
		     unsigned int, augmented, augment_recompute)

static void insert_augmented(struct test_node *node, struct rb_root *root)
{
	struct rb_node **new = &root->rb_node, *rb_parent = NULL;
	unsigned int key = node->key;
	unsigned int val = node->val;
	struct test_node *parent;

	while (*new) {
		rb_parent = *new;
		parent = rb_entry(rb_parent, struct test_node, rb);
		if (parent->augmented < val)
			parent->augmented = val;
		if (key < parent->key)
			new = &parent->rb.rb_left;
		else
			new = &parent->rb.rb_right;
	}

	node->augmented = val;
	rb_link_node(&node->rb, rb_parent, new);
	rb_insert_augmented(&node->rb, root, &augment_callbacks);
}

static void erase_augmented(struct test_node *node, struct rb_root *root)
{
	rb_erase_augmented(&node->rb, root, &augment_callbacks);
}

/**************************test for rbtree end****************/


/**************************test for kfifo begin****************/

struct kfifo_rec_ptr_1 test_kfifo;
#define FIFO_SIZE	1280

static const char *expected_result[] = {
	"a",
	"bb",
	"ccc",
	"dddd",
	"eeeee",
	"ffffff",
	"ggggggg",
	"hhhhhhhh",
	"iiiiiiiii",
	"jjjjjjjjjj",
};

static int testfunc(void)
{
	char		buf[100];
	unsigned int	i;
	unsigned int	ret;
	struct { unsigned char buf[6]; } hello = { "hello" };

	printf("record fifo test start\n");

	kfifo_in(&test_kfifo, &hello, sizeof(hello));

	/* show the size of the next record in the fifo */
	printf("fifo peek len: %u\n", kfifo_peek_len(&test_kfifo));

	/* put in variable length data */
	for (i = 0; i < 10; i++) {
		memset(buf, 'a' + i, i + 1);
		kfifo_in(&test_kfifo, buf, i + 1);
	}

	/* skip first element of the fifo */
	printf("skip 1st element\n");
	kfifo_skip(&test_kfifo);

	printf("fifo len: %u\n", kfifo_len(&test_kfifo));

	/* show the first record without removing from the fifo */
	ret = kfifo_out_peek(&test_kfifo, buf, sizeof(buf));
	if (ret)
		printf("%.*s\n", ret, buf);

	/* check the correctness of all values in the fifo */
	i = 0;
	while (!kfifo_is_empty(&test_kfifo)) {
		ret = kfifo_out(&test_kfifo, buf, sizeof(buf));
		buf[ret] = '\0';
		printf("item = %.*s\n", ret, buf);
		if (strcmp(buf, expected_result[i++])) {
			printf("value mismatch: test failed\n");
			return -EIO;
		}
	}
	if (i != ARRAY_SIZE(expected_result)) {
		printf("size mismatch: test failed\n");
		return -EIO;
	}
	printf("test passed\n");

	return 0;
}
/**************************test for kfifo end****************/


/***************************test for uthash begin******************/
#define HASH_BLOOM
#undef HASH_BLOOM


#define HASH_FUNCTION
#undef HASH_FUNCTION

#define HASH_DEBUG
#undef HASH_DEBUG

#define HASH_EMIT_KEYS
#undef HASH_EMIT_KEYS




#include<uthash.h>



struct my_struct
{  
	int id; /* key */  
	char name[10];  
	UT_hash_handle hh; /* makes this structure hashable */  
};  

struct my_struct *users = NULL; 



int add_user(int user_id, char *name) 
{  
	struct my_struct *s;  
	s = uthash_malloc(sizeof(struct my_struct));  
	s->id = user_id;  
	strcpy(s->name, name);  
	HASH_ADD_INT( users, id, s ); /* id: name of key field */  
	return 0;
}  


void delete_user(struct my_struct *user) 
{  
	HASH_DEL( users, user); /* user: pointer to deletee */  
	uthash_free(user,sizeof(*user)); /* optional; it’s up to you! */  
}


struct my_struct *find_user(int user_id) 
{  
	struct my_struct *s;  
	HASH_FIND_INT( users, &user_id, s ); /* s: output pointer */  
	return s;  
} 

int name_sort(struct my_struct *a, struct my_struct *b) 
{  
	return strcmp(a->name,b->name);  
}  
int id_sort(struct my_struct *a, struct my_struct *b) 
{  
	return (a->id - b->id);  
}  
void sort_by_name() 
{  
	HASH_SORT(users, name_sort);  
}  
void sort_by_id()
{  
	HASH_SORT(users, id_sort);  
}  



static char * names_all[]=
{
	"jason",
	"jim",
	"lily",
	"jinpeng.he"
};





/************************test for uthash end **********************/



int main(int argc, char*argv[])
{
	int i,j;
	unsigned long long time1,time2;
	unsigned long long time_diff;

	/**************************test for rbtree begin****************/

	init();
	time1 = time(NULL);
	for (i = 0; i < PERF_LOOPS; i++) 
	{
		for (j = 0; j < NODES; j++)
		insert(nodes + j, &root);
		for (j = 0; j < NODES; j++)
		erase(nodes + j, &root);
	}
	time2 = time(NULL);
	time_diff = time2 - time1;
	printf(" -> %llu cycles\n", time_diff);
	
	init();
	time1 = time(NULL);

	for (i = 0; i < PERF_LOOPS; i++) {
		for (j = 0; j < NODES; j++)
			insert_augmented(nodes + j, &root);
		for (j = 0; j < NODES; j++)
			erase_augmented(nodes + j, &root);
	}
	time2 = time(NULL);
	time_diff = time2 - time1;
	printf(" -> %llu cycles\n", (unsigned long long)time_diff);
	/**************************test for rbtree end****************/
	
	/**************************test for kfifo begin****************/
	int ret;
	ret = kfifo_alloc(&test_kfifo, FIFO_SIZE);
	if (ret) {
		printf("error kfifo_alloc\n");
		return ret;
	}
	if (testfunc() < 0) {
		kfifo_free(&test_kfifo);
		return -EIO;
	}
	kfifo_free(&test_kfifo);

	/**************************test for kfifo end****************/

	
	/**************************test for kfifo begin****************/
	//向哈希表添加元素
	for(i=0;i<ARRAY_SIZE(names_all);i++)
	{
		add_user(i, names_all[i]);
	}
	//查找元素
	for(i=0;i<ARRAY_SIZE(names_all);i++)
	{
		struct my_struct*s= find_user(i);
		printf("find-->user id %d: name %s\n", s->id, s->name);
	}
	//遍历元素
	struct my_struct *uel, *u_tmp;
	HASH_ITER(hh, users, uel, u_tmp)
	{
		printf("user id %d: name %s\n", uel->id, uel->name);
	}
	
	/**************************test for kfifo end****************/
	printf("hello world\n");
	return 0;
}

