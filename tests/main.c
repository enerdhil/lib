#include "test.h"

int		main(void) {
	u32_t	ret;

	register_infos_tests();

	m_info("Beginning tests...\n");
	ret = test_all();
	test_free();

	return ret;
}
