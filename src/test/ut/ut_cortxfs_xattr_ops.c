/*
 * Filename: cortxfs_xattr_ops.c
 * Description: Implementation tests for xattr
 *
 * Copyright (c) 2020 Seagate Technology LLC and/or its Affiliates
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * For any questions about this software or licensing,
 * please email opensource@seagate.com or cortx-questions@seagate.com. 
 */

#include "cortxfs_xattr_ops.h"
#include <sys/xattr.h> /* XATTR_CREATE */

/**
 * Test to set new (non-existing) xattr
 * Description: Set new (non-existing) xattr for file.
 * Strategy:
 *  1. Set xattr
 *  2. Verify setting xattr by getting xattr value
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. xattr recieved from getxattr should match set xattr.
 */
void set_nonexist_xattr(void **state)
{
	int rc = 0;
	struct ut_cfs_params *ut_cfs_obj = ENV_FROM_STATE(state);

	char *xattr_name = "set.nonexist.xattr", *xattr_val = "1234567890";
	size_t val_size = strlen(xattr_val), get_val_size = XATTR_VAL_SIZE_MAX;

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name, xattr_val,
				val_size, XATTR_CREATE);

	ut_assert_int_equal(rc, 0);

	char buf[XATTR_VAL_SIZE_MAX] = {0};

	rc = cfs_getxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name, buf,
				&get_val_size);

	ut_assert_int_equal(rc, 0);
	ut_assert_int_equal(get_val_size, val_size);

	rc = memcmp(xattr_val, buf, strlen(xattr_val));

	ut_assert_int_equal(rc, 0);
}

/**
 * Setup for set existing xattr test
 * Description: Set new xattr for file.
 * Strategy:
 *  1. Set xattr
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. set xattr operation should be successful
 */
int set_exist_xattr_setup(void **state)
{
	int rc = 0;
	char *xattr_name = "set.exist.xattr", *xattr_val = "1234567890";
	size_t val_size = strlen(xattr_val);

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	ut_xattr_obj->xattr[0] = xattr_name;

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name, xattr_val,
				val_size, XATTR_CREATE);

	ut_assert_int_equal(rc, 0);

	return rc;
}

/**
 * Test to set existing xattr
 * Description: Set existing xattr for file.
 * Strategy:
 *  1. Set existing xattr
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. setxattr operation should fail with error EEXIST
 */
void set_exist_xattr(void **state)
{
	int rc = 0;
	char *xattr_new_val = "1234567890";
	size_t val_size = strlen(xattr_new_val);

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	char * xattr_name = ut_xattr_obj->xattr[0];

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name,
				xattr_new_val, val_size, XATTR_CREATE);

	ut_assert_int_equal(rc, -EEXIST);
}

/**
 * Test to replace new (non-existing) xattr
 * Description: Replace new (non-existing) xattr for file.
 * Strategy:
 *  1. Set new (non-existing) xattr with flag XATTR_REPLACE
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. Setxattr operation should fail with error ENOENT
 */
void replace_nonexist_xattr(void **state)
{
	int rc = 0;
	char *xattr_name = "replace.nonexist.xattr", *xattr_val = "123456789";
	size_t val_size = strlen(xattr_val);

	struct ut_cfs_params *ut_cfs_obj = ENV_FROM_STATE(state);

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name, xattr_val,
				val_size, XATTR_REPLACE);

	ut_assert_int_equal(rc, -ENOENT);
}

/**
 * Setup for replace existing xattr test
 * Description: Set new xattr for file.
 * Strategy:
 *  1. Set xattr
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. setxattr operation should be successful
 */
int replace_exist_xattr_setup(void **state)
{
	int rc = 0;
	char *xattr_name = "replace.exist.xattr", *xattr_val = "1234567890";

	size_t val_size = strlen(xattr_val);

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	ut_xattr_obj->xattr[0] = xattr_name;

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name, xattr_val,
				val_size, XATTR_CREATE);

	ut_assert_int_equal(rc, 0);

	return rc;
}

/**
 * Test to replace existing xattr
 * Description: Replace existing xattr for file.
 * Strategy:
 *  1. Replace existing xattr
 *  2. Verify replace xattr by getting xattr value
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. setxattr operation with flag XATTR_REPLACE should be successful
 */
void replace_exist_xattr(void **state)
{
	int rc = 0;
	char *xattr_new_val = "1234567890";

	size_t val_size = strlen(xattr_new_val),
		get_val_size = XATTR_VAL_SIZE_MAX;

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, ut_xattr_obj->xattr[0],
				xattr_new_val, val_size, XATTR_REPLACE);

	ut_assert_int_equal(rc, 0);

	char buf[XATTR_VAL_SIZE_MAX] = {0};

	rc = cfs_getxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, ut_xattr_obj->xattr[0],
				buf, &get_val_size);

	ut_assert_int_equal(rc, 0);
	ut_assert_int_equal(get_val_size, val_size);

	rc = memcmp(xattr_new_val, buf, strlen(xattr_new_val));

	ut_assert_int_equal(rc, 0);
}

/**
 * Setup for get existing xattr test
 * Description: Set xattr for file.
 * Strategy:
 *  1. Set xattr
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. Setxattr operation should be successful
 */
int get_exist_xattr_setup(void **state)
{
	int rc = 0;

	char *xattr_name = "get.exist.xattr", *xattr_set_val = "1234567890";
	size_t xattr_set_size = strlen(xattr_set_val);

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	ut_xattr_obj->xattr[0] = xattr_name;
	ut_xattr_obj->xattr[1] = xattr_set_val;

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name,
				xattr_set_val, xattr_set_size, XATTR_CREATE);

	ut_assert_int_equal(rc, 0);

	return rc;
}

/**
 * Test to get existing xattr
 * Description: Get existing xattr for file.
 * Strategy:
 *  1. Get xattr value
 *  2. Verify output
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. value from getxattr should match with set value.
 */
void get_exist_xattr(void **state)
{
	int rc = 0;

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	size_t xattr_set_size = strlen(ut_xattr_obj->xattr[1]);

	char xattr_val[XATTR_VAL_SIZE_MAX] = {0};
	size_t val_size = XATTR_VAL_SIZE_MAX;

	rc  = cfs_getxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, ut_xattr_obj->xattr[0],
				xattr_val, &val_size);

	ut_assert_int_equal(rc, 0);
	ut_assert_int_equal(val_size, xattr_set_size);

	rc = memcmp(ut_xattr_obj->xattr[1], xattr_val, xattr_set_size);

	ut_assert_int_equal(rc, 0);
}

/**
 * Test to get nonexisting xattr
 * Description: Get nonexisting xattr for file.
 * Strategy:
 *  1. Get xattr value
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. getxattr soperation should fail with error ENOENT
 */
void get_nonexist_xattr(void **state)
{
	int rc = 0;
	char *xattr_name = "get.nonexist.xattr";

	char xattr_val[XATTR_VAL_SIZE_MAX] = {0};
	size_t val_size = XATTR_VAL_SIZE_MAX;

	struct ut_cfs_params *ut_cfs_obj = ENV_FROM_STATE(state);

	rc = cfs_getxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name, xattr_val,
				&val_size);

	ut_assert_int_equal(rc, -ENOENT);
}

/**
 * Setup for remove existing xattr test
 * Description: Set xattr for file.
 * Strategy:
 *  1. Set xattr
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. Setxattr operation should be successful.
 */
int remove_exist_xattr_setup(void **state)
{
	int rc = 0;
	char *xattr_name = "remove.exist.xattr", *xattr_set_val = "1234567890";
	size_t xattr_set_size = strlen(xattr_set_val);

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	ut_xattr_obj->xattr[0] = xattr_name;

	rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name,
				xattr_set_val, xattr_set_size, XATTR_CREATE);
	ut_assert_int_equal(rc, 0);

	return rc;
}

/**
 * Test to remove existing xattr
 * Description: Remove existing xattr for file.
 * Strategy:
 *  1. Remove xattr
 *  2. Verify xattr remove operation by getting xattr
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. xattr remove operation should be successful
 *  3. getxattr should fail with error ENOENT.
 */
void remove_exist_xattr(void **state)
{
	int rc = 0;

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	rc = cfs_removexattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, ut_xattr_obj->xattr[0]);

	ut_assert_int_equal(rc, 0);

	char xattr_val[XATTR_VAL_SIZE_MAX] = {0};
	size_t val_size = XATTR_VAL_SIZE_MAX;

	rc = cfs_getxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, ut_xattr_obj->xattr[0],
				xattr_val, &val_size);

	ut_assert_int_equal(rc, -ENOENT);
}

/**
 * Test to remove nonexisting xattr
 * Description: Remove nonexisting xattr for file.
 * Strategy:
 *  1. Remove xattr
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. xattr remove operation should fail with error ENOENT
 */
void remove_nonexist_xattr(void **state)
{
	int rc = 0;
	char *xattr_name = "remove.nonexist.xattr";

	struct ut_cfs_params *ut_cfs_obj = ENV_FROM_STATE(state);

	rc = cfs_removexattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
				&ut_cfs_obj->file_inode, xattr_name);

	ut_assert_int_equal(rc, -ENOENT);
}

/**
 * Setup for list xattr test
 * Description: Set xattrs for file.
 * Strategy:
 *  1. Create a file.
 *  2. Set multiple xattr for file.
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. Setxattr operations should be successful.
 */
int listxattr_test_setup(void **state)
{
	char xattr_name[][4096] = { "xattr.checksum", "xattr.attribute"},
		xattr_set_val[][XATTR_VAL_SIZE_MAX] = {"1234567890", "999999999"},
		*file_name = "test.listxattr";
	size_t xattr_set_size[] = {strlen(xattr_set_val[0]),
					strlen(xattr_set_val[1])};
	cfs_ino_t file_inode = 0LL;

	int rc = 0, i,
		xattr_set_cnt = sizeof(xattr_set_size)/sizeof(xattr_set_size[0]);

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	ut_xattr_obj->xattr[0] = file_name;
	for (i = 0; i<xattr_set_cnt; i++) {
		ut_xattr_obj->xattr[i+1] = xattr_name[i];
	}

	rc = cfs_creat(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
			&ut_cfs_obj->current_inode, file_name, 0755, &file_inode);

	ut_assert_int_equal(rc, 0);


	for (i = 0; i<xattr_set_cnt; i ++) {
		rc = cfs_setxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
					&file_inode, xattr_name[i],
					xattr_set_val[i], xattr_set_size[i],
					XATTR_CREATE);

		ut_assert_int_equal(rc, 0);
	}

	return rc;
}

/**
 * Test to list xattr
 * Description: List xattr for file.
 * Strategy:
 *  1. List xattr
 *  2. xattrs values received from list xattr should match with set values
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. list xattr operation should be successful
 *  3. xattr values set and recieved from list xattr should match.
 */
void listxattr_test(void **state)
{
	int rc = 0;

	cfs_ino_t file_inode = 0LL;

	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	rc = cfs_lookup(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
			&ut_cfs_obj->parent_inode, ut_xattr_obj->xattr[0],
			&file_inode);

	size_t count, buf_size = XATTR_VAL_SIZE_MAX;
	char *buf;

	buf = malloc(buf_size * sizeof(*buf));

	rc = cfs_listxattr(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred, &file_inode,
				buf, &count, &buf_size);

	ut_assert_int_equal(rc, 0);

	ut_assert_int_equal(2, count);

	char *p = buf;
	int len;

	int i ;

	for (i = 0; i<count; i++) {
		len = strlen(ut_xattr_obj->xattr[i+1]);
		rc = memcmp(p, ut_xattr_obj->xattr[i+1], len);

		ut_assert_int_equal(rc, 0);

		p += len;
		p++;
	}
}

/**
 * Teardown for list xattr test
 * Description: Delete file.
 * Strategy:
 * 1. Delete file
 * Expected behavior:
 *  1. No errors from CORTXFS API.
 *  2. File deletion should be successful.
 */
int listxattr_test_teardown(void **state)
{
	int rc = 0;
	struct ut_xattr_env *ut_xattr_obj = XATTR_ENV_FROM_STATE(state);
	struct ut_cfs_params *ut_cfs_obj = &ut_xattr_obj->ut_cfs_obj;

	cfs_unlink(ut_cfs_obj->cfs_fs, &ut_cfs_obj->cred,
		&ut_cfs_obj->current_inode, NULL, ut_xattr_obj->xattr[0]);

	ut_assert_int_equal(rc, 0);

	return rc;
}

