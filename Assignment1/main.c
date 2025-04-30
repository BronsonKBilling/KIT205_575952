#include "record.h"
#include "linked_list.h"
#include "avl_tree.h"
#include "database.h"

int main() {
	test_record();
	test_list();
    test_avl();
	test_database();
	evaluate_database();
	return 0;
}