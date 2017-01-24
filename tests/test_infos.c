#include "test.h"

TEST(program_name_1) {
	TEST_ASSERT((get_program_name() == NULL), "Wrong program name init");
	return TEST_SUCCESS;
}

TEST(program_name_2) {
	set_program_name("Hey !");
	TEST_ASSERT(strcmp(get_program_name(), "Hey !") == 0, "Wrong program_name return");
	return TEST_SUCCESS;
}

TEST(program_name_3) {
	TEST_ASSERT(strcmp(get_program_name(), "Hey !") == 0, "Wrong program_name return");
	set_program_name("Yo !");
	TEST_ASSERT(strcmp(get_program_name(), "Yo !") == 0, "Wrong program_name return");
	return TEST_SUCCESS;
}

TEST(program_name_4) {
	TEST_ASSERT(strcmp(get_program_name(), "Yo !") == 0, "Wrong program_name return");
	set_program_name("");
	TEST_ASSERT((get_program_name() == NULL), "Wrong program_name return");
	set_program_name(NULL);
	TEST_ASSERT((get_program_name() == NULL), "Wrong program_name return");
	return TEST_SUCCESS;
}

TEST(version_1) {
	TEST_ASSERT((get_version() == NULL), "Wrong version init");
	return TEST_SUCCESS;
}

TEST(version_2) {
	set_version("Hey !");
	TEST_ASSERT(strcmp(get_version(), "Hey !") == 0, "Wrong version return");
	return TEST_SUCCESS;
}

TEST(version_3) {
	TEST_ASSERT(strcmp(get_version(), "Hey !") == 0, "Wrong version return");
	set_version("Yo !");
	TEST_ASSERT(strcmp(get_version(), "Yo !") == 0, "Wrong version return");
	return TEST_SUCCESS;
}

TEST(version_4) {
	TEST_ASSERT(strcmp(get_version(), "Yo !") == 0, "Wrong version return");
	set_version("");
	TEST_ASSERT((get_version() == NULL), "Wrong version return");
	set_version(NULL);
	TEST_ASSERT((get_version() == NULL), "Wrong version return");
	return TEST_SUCCESS;
}

TEST(maintainer_1) {
	TEST_ASSERT((get_maintainer() == NULL), "Wrong maintainer init");
	return TEST_SUCCESS;
}

TEST(maintainer_2) {
	set_maintainer("Hey !");
	TEST_ASSERT(strcmp(get_maintainer(), "Hey !") == 0, "Wrong maintainer return");
	return TEST_SUCCESS;
}

TEST(maintainer_3) {
	TEST_ASSERT(strcmp(get_maintainer(), "Hey !") == 0, "Wrong maintainer return");
	set_maintainer("Yo !");
	TEST_ASSERT(strcmp(get_maintainer(), "Yo !") == 0, "Wrong maintainer return");
	return TEST_SUCCESS;
}

TEST(maintainer_4) {
	TEST_ASSERT(strcmp(get_maintainer(), "Yo !") == 0, "Wrong maintainer return");
	set_maintainer("");
	TEST_ASSERT((get_maintainer() == NULL), "Wrong maintainer return");
	set_maintainer(NULL);
	TEST_ASSERT((get_maintainer() == NULL), "Wrong maintainer return");
	return TEST_SUCCESS;
}


void	register_infos_tests(void) {
	reg_test("m_infos", program_name_1);
	reg_test("m_infos", program_name_2);
	reg_test("m_infos", program_name_3);
	reg_test("m_infos", program_name_4);
	reg_test("m_infos", version_1);
	reg_test("m_infos", version_2);
	reg_test("m_infos", version_3);
	reg_test("m_infos", version_4);
	reg_test("m_infos", maintainer_1);
	reg_test("m_infos", maintainer_2);
	reg_test("m_infos", maintainer_3);
	reg_test("m_infos", maintainer_4);
}
