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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <aul.h>
#include <aul_service.h>
#include <vconf.h>
#include <ail.h>
#include <dlog.h>

#include <app_manager_private.h>
#include <app_manager.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_APP_MANAGER"

#define MENU_PKG_VCONF_KEY "memory/menuscreen/desktop"

#define EVENT_CREATE "create"
#define EVENT_UPDATE "update"
#define EVENT_DELETE "delete"

typedef struct {
	app_manager_app_running_cb cb;
	void *user_data;
	bool *foreach_break;
} running_apps_foreach_cb_context;

typedef struct {
	app_manager_app_installed_cb cb;
	void *user_data;
} installed_apps_foreach_cb_context;

static app_manager_app_list_changed_cb app_list_changed_cb = NULL;

static void app_manager_meun_pkg_changed(keynode_t* node, void *data);

static int foreach_running_app_cb_broker(const aul_app_info * appcore_app_info, void *appcore_user_data)
{
	ail_appinfo_h handle;
	ail_error_e ret;
	bool task_manage = false;
	running_apps_foreach_cb_context *foreach_cb_context = NULL;

	if (appcore_app_info == NULL || appcore_user_data == NULL) 
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid callback context", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return 0;
	}

	ret = ail_package_get_appinfo(appcore_app_info->pkg_name, &handle);
	if (ret != AIL_ERROR_OK)
	{
		LOGE("[%s] DB_FAILED(0x%08x) : failed to get the app-info", __FUNCTION__, APP_MANAGER_ERROR_DB_FAILED);
		return 0;
	}

	// do not call callback function when X-SLP-TaskManage is set to false
	ret = ail_appinfo_get_bool(handle, AIL_PROP_X_SLP_TASKMANAGE_BOOL, &task_manage);

	ail_package_destroy_appinfo(handle);

	if (ret != AIL_ERROR_OK || task_manage == false)
	{
		return 0;
	}

	foreach_cb_context = (running_apps_foreach_cb_context *)appcore_user_data;

	if (foreach_cb_context->cb != NULL && *(foreach_cb_context->foreach_break) == false)
	{
		if (foreach_cb_context->cb(appcore_app_info->pkg_name, foreach_cb_context->user_data) == false)
		{
			*(foreach_cb_context->foreach_break) = true;
		}
	}

	return 0;
}

static ail_cb_ret_e foreach_installed_app_cb_broker(const ail_appinfo_h appinfo, void *ail_user_data)
{
	installed_apps_foreach_cb_context *foreach_cb_context = NULL;
	char *package;

	if (appinfo == NULL || ail_user_data == NULL)
	{
		return AIL_CB_RET_CANCEL;
	}

	foreach_cb_context = (installed_apps_foreach_cb_context *)ail_user_data;

	ail_appinfo_get_str(appinfo, AIL_PROP_PACKAGE_STR, &package);

	if (foreach_cb_context->cb(package, foreach_cb_context->user_data)  == false)
	{
		return AIL_CB_RET_CANCEL;
	}
	
	return AIL_CB_RET_CONTINUE;

}


static int app_manager_ail_error_handler(ail_error_e ail_error, const char *func)
{
	int error_code;
	char *error_msg;

	switch(ail_error)
	{
		case AIL_ERROR_FAIL:
			error_code = APP_MANAGER_ERROR_INVALID_PARAMETER;
			error_msg = "INVALID_PARAMETER";
			break;

		case AIL_ERROR_DB_FAILED:
			error_code = APP_MANAGER_ERROR_DB_FAILED;
			error_msg = "DB_FAILED";
			break;

		case AIL_ERROR_OUT_OF_MEMORY:
			error_code = APP_MANAGER_ERROR_OUT_OF_MEMORY;
			error_msg = "OUT_OF_MEMORY";
			break;

		case AIL_ERROR_INVALID_PARAMETER:
			error_code = APP_MANAGER_ERROR_INVALID_PARAMETER;
			error_msg = "INVALID_PARAMETER";
			break;
		
		case AIL_ERROR_OK:
			error_code = APP_MANAGER_ERROR_NONE;
			break;
			
		default:
			error_code = APP_MANAGER_ERROR_INVALID_PARAMETER;
			error_msg = "INVALID_PARAMETER";
	}

	if (error_code != APP_MANAGER_ERROR_NONE)
	{
		LOGE("[%s] %s(0x%08x)", func, error_msg, error_code);
	}

	return error_code;
}


int app_manager_foreach_app_running(app_manager_app_running_cb callback, void *user_data)
{
	bool foreach_break = false;

	running_apps_foreach_cb_context foreach_cb_context = {
		.cb = callback,
		.user_data = user_data,
		.foreach_break = &foreach_break
	};

	if (callback == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid callback", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	aul_app_get_running_app_info(foreach_running_app_cb_broker, &foreach_cb_context);

	return APP_MANAGER_ERROR_NONE;
}

int app_manager_is_running(const char *package, bool *is_running)
{
	if (package == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid package", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	if (is_running == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid output param", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	*is_running = aul_app_is_running(package);

	return APP_MANAGER_ERROR_NONE;
}

 int app_manager_foreach_app_installed(app_manager_app_installed_cb callback, void *user_data)
{
	ail_filter_h filter;
	ail_error_e ret;

	if (callback == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid callback", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	ret = ail_filter_new(&filter);
	if (ret != AIL_ERROR_OK)
	{
		return app_manager_ail_error_handler(ret, __FUNCTION__);
	}

	// Provide visible application to 3rd party developer
	ret = ail_filter_add_bool(filter, AIL_PROP_NODISPLAY_BOOL, false);
	if (ret != AIL_ERROR_OK)
	{
		ail_filter_destroy(filter);
		return app_manager_ail_error_handler(ret, __FUNCTION__);
	}

	// Provide task manageable app only to 3rd party developer
	ret = ail_filter_add_bool(filter, AIL_PROP_X_SLP_TASKMANAGE_BOOL, true);
	if (ret != AIL_ERROR_OK)
	{
		ail_filter_destroy(filter);
		return app_manager_ail_error_handler(ret, __FUNCTION__);
	}

	installed_apps_foreach_cb_context foreach_cb_context = {
		.cb = callback,
		.user_data = user_data,
	};

	ail_filter_list_appinfo_foreach(filter, foreach_installed_app_cb_broker, &foreach_cb_context);

	ail_filter_destroy(filter);
	
	return APP_MANAGER_ERROR_NONE;
}

static int app_manager_get_appinfo(const char *package, ail_prop_str_e property, char **value)
{
	ail_error_e ail_error;
	ail_appinfo_h appinfo;
	char *appinfo_value;
	char *appinfo_value_dup;

	ail_error = ail_package_get_appinfo(package, &appinfo);
	if (ail_error != AIL_ERROR_OK)
	{
		return app_manager_ail_error_handler(ail_error, __FUNCTION__);
	}

	ail_error = ail_appinfo_get_str(appinfo, property, &appinfo_value);
	if (ail_error != AIL_ERROR_OK)
	{
		ail_package_destroy_appinfo(appinfo);
		return app_manager_ail_error_handler(ail_error, __FUNCTION__);
	}

	appinfo_value_dup = strdup(appinfo_value);

	ail_package_destroy_appinfo(appinfo);

	if (appinfo_value_dup == NULL)
	{
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, APP_MANAGER_ERROR_OUT_OF_MEMORY);
		return APP_MANAGER_ERROR_OUT_OF_MEMORY;
	}

	*value = appinfo_value_dup;
	
	return APP_MANAGER_ERROR_NONE;
}

int app_manager_get_app_name(const char *package, char** name)
{
	if (package == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid package", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	return app_manager_get_appinfo(package, AIL_PROP_NAME_STR, name);
}
 
int app_manager_get_app_icon_path(const char *package, char** icon_path)
{
	if (package == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid package", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	return app_manager_get_appinfo(package, AIL_PROP_ICON_STR, icon_path);
}

int app_manager_get_app_version(const char *package, char** version)
{
	if (package == NULL)
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid package", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	return app_manager_get_appinfo(package, AIL_PROP_VERSION_STR, version);
}

int app_manager_set_app_list_changed_cb(app_manager_app_list_changed_cb callback, void* user_data)
{
       if (callback == NULL) 
	{
		LOGE("[%s] INVALID_PARAMETER(0x%08x) : invalid callback", __FUNCTION__, APP_MANAGER_ERROR_INVALID_PARAMETER);
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	if (app_list_changed_cb == NULL)
	{
		vconf_notify_key_changed(MENU_PKG_VCONF_KEY, app_manager_meun_pkg_changed, user_data);
	}

	app_list_changed_cb = callback;

	return APP_MANAGER_ERROR_NONE;
}

int app_manager_unset_app_list_changed_cb()
{	
	if (app_list_changed_cb != NULL)
	{
		if (vconf_ignore_key_changed(MENU_PKG_VCONF_KEY, app_manager_meun_pkg_changed))
		{
			LOGE("[%s] DB_FAILED(0x%08x)", __FUNCTION__, APP_MANAGER_ERROR_DB_FAILED);
			return APP_MANAGER_ERROR_DB_FAILED;
		}
	}

	app_list_changed_cb = NULL;

	return APP_MANAGER_ERROR_NONE;
}

static void app_manager_meun_pkg_changed(keynode_t* node, void *data)
{
	char *pkg_event;
	char type[10];
	char package[1024];

	pkg_event = vconf_get_str(vconf_keynode_get_name(node));
	if(!pkg_event) {
		LOGE("[%s] failed to get the package event");
		return;
	}

	if (sscanf(pkg_event, "%10[^:]:%1023s", type, package) != 2)
	{
		LOGE("[%s] failed to parse the package event format : [%s], [%s]", __FUNCTION__, type, package);
	}


	if(app_list_changed_cb == NULL)
	{
		return;
	}
	
	if(!strcasecmp(type, EVENT_CREATE))
	{
		// A new application has been installed.
		app_list_changed_cb(APP_MANAGER_EVENT_INSTALLED, package, data);
	}
	else if( !strcasecmp(type, EVENT_UPDATE))
	{
		// An existing application has been updated.
		app_list_changed_cb(APP_MANAGER_EVENT_UPDATED , package, data);
	}

	else if( !strcasecmp(type, EVENT_DELETE))
	{
		// An existing application has been uninstalled.
		app_list_changed_cb(APP_MANAGER_EVENT_UNINSTALLED  , package, data);
	}
	else
	{
		LOGE("[%s] invalid event : type(%s)", __FUNCTION__, type);
	}

}

