/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tet_api.h>
#include <app_manager.h>

#include <stdbool.h>
#include <dlog.h>

#undef LOG_TAG
#define LOG_TAG "APPLICATION_MANAGER_TC"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_app_manager_app_context_clone_p(void);
static void utc_app_manager_app_context_clone_n(void);
static void utc_app_manager_app_context_destroy_p(void);
static void utc_app_manager_app_context_destroy_n(void);
static void utc_app_manager_app_context_get_appid_p(void);
static void utc_app_manager_app_context_get_appid_n(void);
static void utc_app_manager_app_context_get_package_p(void);
static void utc_app_manager_app_context_get_package_n(void);
static void utc_app_manager_app_context_get_pid_p(void);
static void utc_app_manager_app_context_get_pid_n(void);
static void utc_app_manager_app_context_is_equal_p(void);
static void utc_app_manager_app_context_is_equal_n(void);
static void utc_app_manager_app_context_is_terminated_p(void);
static void utc_app_manager_app_context_is_terminated_n(void);

struct tet_testlist tet_testlist[] = {
	{ utc_app_manager_app_context_clone_p, 1 },
	{ utc_app_manager_app_context_clone_n, 2 },
	{ utc_app_manager_app_context_destroy_p, 1},
	{ utc_app_manager_app_context_destroy_n, 2},
	{ utc_app_manager_app_context_get_appid_p, 1},
	{ utc_app_manager_app_context_get_appid_n, 2},
	{ utc_app_manager_app_context_get_package_p, 1},
	{ utc_app_manager_app_context_get_package_n, 2},
	{ utc_app_manager_app_context_get_pid_p, 1},
	{ utc_app_manager_app_context_get_pid_n, 2},
	{ utc_app_manager_app_context_is_equal_p, 1},
	{ utc_app_manager_app_context_is_equal_n, 2},
	{ utc_app_manager_app_context_is_terminated_p, 1},
	{ utc_app_manager_app_context_is_terminated_n, 2},
	{ NULL, 0 },
};

static void startup(void)
{
	/* start of TC */
}

static void cleanup(void)
{
	/* end of TC */
}


static void utc_app_manager_app_context_clone_p(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	app_context_h app_context_cloned = NULL;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_clone_p", "failed");
		return;
	}
	ret = app_context_clone(&app_context_cloned, app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_clone_p", "failed");
		return;
	}
	dts_pass("utc_app_manager_app_context_clone_p", "passed");

}

static void utc_app_manager_app_context_clone_n(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	app_context_h app_context_cloned = NULL;
	ret = app_context_clone(&app_context_cloned, app_context);
	if (ret == APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_clone_n", "failed");
		return;
	}
	dts_pass("utc_app_manager_app_context_clone_n", "passed");
}

static void utc_app_manager_app_context_destroy_p(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_destroy_p", "failed");
		return;
	}
	ret = app_context_destroy(app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_destroy_p", "failed");
		return;
	}
	dts_pass("utc_app_manager_app_context_destroy_p", "passed");
}

static void utc_app_manager_app_context_destroy_n(void)
{
	if (app_context_destroy(NULL) == APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_destroy_n", "failed");
		return;
	}
	dts_pass("utc_app_manager_app_context_destroy_n", "passed");
}

static void utc_app_manager_app_context_get_appid_p(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_get_appid_p", "failed");
		return;
	}
	char *app_id;
	ret = app_context_get_app_id(app_context, &app_id);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_get_appid_p", "failed");
		return;
	}
	dts_pass("utc_app_manager_app_context_get_appid_p", "passed");
}

static void utc_app_manager_app_context_get_appid_n(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	char *app_id;
	ret = app_context_get_app_id(app_context, &app_id);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_app_context_get_appid_n", "passed");
		return;
	}
	dts_fail("utc_app_manager_app_context_get_appid_n", "failed");

}

static void utc_app_manager_app_context_get_package_p(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_get_package_p", "failed");
		return;
	}
	char *package;
	ret = app_context_get_package(app_context, &package);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_get_package_p", "failed");
		return;
	}
	dts_pass("utc_app_manager_app_context_get_package_p", "passed");

}

static void utc_app_manager_app_context_get_package_n(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	char *package;
	ret = app_context_get_package(app_context, &package);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_app_context_get_package_n", "passed");
		return;
	}
	dts_fail("utc_app_manager_app_context_get_package_n", "failed");

}

static void utc_app_manager_app_context_get_pid_p(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_get_pid_p", "failed");
		return;
	}
	int pid = 0;
	ret = app_context_get_pid(app_context, &pid);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_get_pid_p", "failed");
		return;
	}
	dts_pass("utc_app_manager_app_context_get_pid_p", "passed");
}

static void utc_app_manager_app_context_get_pid_n(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	int pid = 0;
	ret = app_context_get_package(app_context, &pid);
	if (ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_app_context_get_pid_n", "passed");
		return;
	}
	dts_fail("utc_app_manager_app_context_get_pid_n", "failed");

}

static void utc_app_manager_app_context_is_equal_p(void)
{
	int ret = 0;
	app_context_h app_context1 = NULL;
	app_context_h app_context2 = NULL;
	bool equal = false;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context1);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_is_equal_p", "failed");
		return;
	}
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context2);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_is_equal_p", "failed");
		return;
	}
	ret = app_context_is_equal(app_context1, app_context2, &equal);
	if (equal == true){
		dts_pass("utc_app_manager_app_context_is_equal_p", "passed");
	}else{
		dts_pass("utc_app_manager_app_context_is_equal_p", "failed");
	}
}

static void utc_app_manager_app_context_is_equal_n(void)
{
	int ret = app_context_is_equal(NULL, NULL, NULL);
	if (ret == APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_is_equal_p", "failed");
	}else{
		dts_pass("utc_app_manager_app_context_is_equal_p", "passed");
	}
}

static void utc_app_manager_app_context_is_terminated_p(void)
{
	int ret = 0;
	app_context_h app_context = NULL;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret != APP_MANAGER_ERROR_NONE){
		//TODO: return as TC init fail
		dts_fail("utc_app_manager_app_context_destroy_p", "failed");
		return;
	}
	bool terminated = false;
	ret = app_context_is_terminated(app_context, &terminated);
	if (ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_app_context_is_terminated_p", "passed");
	}else{
		dts_fail("utc_app_manager_app_context_is_terminated_p", "failed");
	}
}


static void utc_app_manager_app_context_is_terminated_n(void)
{
	int ret = 0;
	bool terminated = false;
	app_context_h app_context = NULL;
	ret = app_context_is_terminated(app_context, &terminated);
	if (ret == APP_MANAGER_ERROR_NONE){
		dts_fail("utc_app_manager_app_context_is_terminated_n", "failed");
	}else{
		dts_pass("utc_app_manager_app_context_is_terminated_n", "passed");
	}
}

