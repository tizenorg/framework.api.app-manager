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

#include <ail.h>
#include <pkgmgr-info.h>
#include <package-manager.h>
#include <dlog.h>

#include <app_info.h>
#include <app_manager.h>
#include <app_manager_private.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "TIZEN_N_APP_MANAGER"

struct app_info_s {
	char *app_id;
	pkgmgrinfo_appinfo_h pkg_app_info;
};

struct app_info_filter_s {
	pkgmgrinfo_appinfo_filter_h pkg_app_info_filter;
};

struct app_info_metadata_filter_s {
	pkgmgrinfo_appinfo_metadata_filter_h pkg_app_info_metadata_filter;
};

typedef struct _foreach_context_{
	app_manager_app_info_cb callback;
	void *user_data;
} foreach_context_s;

typedef struct _foreach_metada_context_{
	app_info_metadata_cb callback;
	void *user_data;
} foreach_metadata_context_s;

static pkgmgr_client *package_event_listener = NULL;
static app_manager_app_info_event_cb app_info_event_cb = NULL;
static void *app_info_event_cb_data = NULL;

static int app_info_convert_str_property(const char *property, char **converted_property)
{
	if (property == NULL)
		return -1;

	if (strcmp(property, PACKAGE_INFO_PROP_APP_ID)==0)
		*converted_property = PMINFO_APPINFO_PROP_APP_ID;

	else if (strcmp(property, PACKAGE_INFO_PROP_APP_TYPE)==0)
		*converted_property = PMINFO_APPINFO_PROP_APP_TYPE;

	else
		return -1;

	return 0;
}

static int app_info_convert_bool_property(const char *property, char **converted_property)
{
	if (property == NULL)
		return -1;

	if (strcmp(property, PACKAGE_INFO_PROP_APP_NODISPLAY)==0)
		*converted_property = PMINFO_APPINFO_PROP_APP_NODISPLAY;

	else if (strcmp(property, PACKAGE_INFO_PROP_APP_TASKMANAGE)==0)
		*converted_property = PMINFO_APPINFO_PROP_APP_TASKMANAGE;

	else
		return -1;

	return 0;
}

int app_info_foreach_app_filter_cb(pkgmgrinfo_appinfo_h handle, void *user_data)
{
	int retval = 0;
	char *appid = NULL;
	app_info_h info = NULL;
	info = calloc(1, sizeof(struct app_info_s));
	if (info == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	foreach_context_s *foreach_context = user_data;
	if (handle == NULL || foreach_context == NULL)
	{
		free(info);
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_get_appid(handle, &appid);
	if (retval < 0)
	{
		free(info);
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	info->app_id = strdup(appid);
	info->pkg_app_info = handle;

	foreach_context->callback(info, foreach_context->user_data);

	return APP_MANAGER_ERROR_NONE;
}

static int app_info_foreach_app_metadata_cb(const char *metadata_key, const char *metadata_value, void *user_data)
{
	foreach_metadata_context_s *foreach_context = user_data;

	if (metadata_value == NULL || foreach_context == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	foreach_context->callback(metadata_key, metadata_value, foreach_context->user_data);

	return APP_MANAGER_ERROR_NONE;
}

static ail_cb_ret_e app_info_foreach_app_info_cb(const ail_appinfo_h ail_app_info, void *cb_data)
{
	foreach_context_s *foreach_context = cb_data;
	app_info_h app_info = NULL;
	char *app_id;
	bool iteration_next = true;

	if (ail_app_info == NULL || foreach_context == NULL)
	{
		app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
		return AIL_CB_RET_CANCEL;
	}

	ail_appinfo_get_str(ail_app_info, AIL_PROP_PACKAGE_STR, &app_id);

	if (app_info_create(app_id, &app_info) == APP_MANAGER_ERROR_NONE)
	{
		iteration_next = foreach_context->callback(app_info, foreach_context->user_data);
		app_info_destroy(app_info);
	}

	if (iteration_next == true)
	{
		return AIL_CB_RET_CONTINUE;
	}
	else
	{
		return AIL_CB_RET_CANCEL;
	}
}

int app_info_foreach_app_info(app_manager_app_info_cb callback, void *user_data)
{
	foreach_context_s foreach_context = {
		.callback = callback,
		.user_data = user_data,
	};

	if (callback == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	ail_filter_list_appinfo_foreach(NULL, app_info_foreach_app_info_cb, &foreach_context);

	return APP_MANAGER_ERROR_NONE;
}

int app_info_get_app_info(const char *app_id, app_info_h *app_info)
{
	return app_info_create(app_id, app_info);
}

int app_info_create(const char *app_id, app_info_h *app_info)
{
	pkgmgrinfo_pkginfo_h pkginfo = NULL;
	pkgmgrinfo_appinfo_h appinfo = NULL;
	app_info_h info = NULL;
	int retval = 0;
	char *main_appid = NULL;

	if (app_id == NULL || app_info == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	info = calloc(1, sizeof(struct app_info_s));
	if (info == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_pkginfo_get_pkginfo(app_id, &pkginfo);
	if (retval < 0) {
		if (pkgmgrinfo_appinfo_get_appinfo(app_id, &appinfo)) {
			free(info);
			return app_manager_error(APP_MANAGER_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
		}

		info->app_id = strdup(app_id);
		info->pkg_app_info = appinfo;
		*app_info = info;
		return APP_MANAGER_ERROR_NONE;
	}

	retval = pkgmgrinfo_pkginfo_get_mainappid(pkginfo, &main_appid);
	if (retval < 0) {
		app_manager_error(APP_MANAGER_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
	}

	if (pkgmgrinfo_appinfo_get_appinfo(main_appid, &appinfo)) {
		free(info);
		return app_manager_error(APP_MANAGER_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
	}

	info->app_id = strdup(main_appid);
	info->pkg_app_info = appinfo;
	*app_info = info;

	pkgmgrinfo_pkginfo_destroy_pkginfo(pkginfo);

	return APP_MANAGER_ERROR_NONE;
}

int app_info_destroy(app_info_h app_info)
{
	if (app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	if (app_info->app_id) {
		free(app_info->app_id);
		app_info->app_id = NULL;
	}
	pkgmgrinfo_appinfo_destroy_appinfo(app_info->pkg_app_info);
	free(app_info);
	return APP_MANAGER_ERROR_NONE;
}

int app_info_get_app_id(app_info_h app_info, char **app_id)
{
	char *app_id_dup;

	if (app_info == NULL || app_id == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_id_dup = strdup(app_info->app_id);

	if (app_id_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*app_id = app_id_dup;

	return APP_MANAGER_ERROR_NONE;
}

int app_info_get_exec(app_info_h app_info, char **exec)
{
	char *val;
	char *app_exec_dup;

	if (app_info == NULL || exec == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_exec(app_info->pkg_app_info, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_exec_dup = strdup(val);
	if (app_exec_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*exec = app_exec_dup;

	return APP_MANAGER_ERROR_NONE;
}

int app_info_get_name(app_info_h app_info, char **label)
{
	return app_info_get_label(app_info, label);
}


int app_info_get_label(app_info_h app_info, char **label)
{
	char *val;
	char *app_label_dup;

	if (app_info == NULL || label == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_label(app_info->pkg_app_info, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_label_dup = strdup(val);
	if (app_label_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*label = app_label_dup;

	return APP_MANAGER_ERROR_NONE;
}

int app_info_get_localed_label(const char *app_id, const char *locale, char **label)
{
	char *val;
	char *app_label_dup;

	if (app_id == NULL || locale == NULL || label == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_localed_label(app_id, locale, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_label_dup = strdup(val);
	if (app_label_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*label = app_label_dup;
	free(val);

	return APP_MANAGER_ERROR_NONE;
}

int app_info_get_icon(app_info_h app_info, char **path)
{
	char *val;
	char *app_icon_dup;

	if (app_info == NULL || path == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_icon(app_info->pkg_app_info, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_icon_dup = strdup(val);
	if (app_icon_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*path = app_icon_dup;

	return APP_MANAGER_ERROR_NONE;
}

int app_info_get_package(app_info_h app_info, char **package)
{
	char *val;
	char *app_package_dup;

	if (app_info == NULL || package == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_pkgname(app_info->pkg_app_info, &val);

	app_package_dup = strdup(val);
	if (app_package_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*package = app_package_dup;

	return APP_MANAGER_ERROR_NONE;
}


int app_info_get_type(app_info_h app_info, char **type)
{
	char *val;
	char *app_type_dup;

	if (app_info == NULL || type == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_apptype(app_info->pkg_app_info, &val);

	app_type_dup = strdup(val);
	if (app_type_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*type = app_type_dup;

	return APP_MANAGER_ERROR_NONE;
}

int app_info_foreach_metadata(app_info_h app_info, app_info_metadata_cb callback, void *user_data)
{
	int retval = 0;

	if (app_info == NULL || callback == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	foreach_metadata_context_s foreach_context = {
		.callback = callback,
		.user_data = user_data,
	};

	retval = pkgmgrinfo_appinfo_foreach_metadata(app_info->pkg_app_info, app_info_foreach_app_metadata_cb, &foreach_context);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_is_nodisplay(app_info_h app_info, bool *nodisplay)
{
	bool val;

	if (app_info == NULL || nodisplay == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	if (pkgmgrinfo_appinfo_is_nodisplay(app_info->pkg_app_info, &val) < 0)
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	*nodisplay = val;
	return APP_MANAGER_ERROR_NONE;
}

int app_info_is_enabled(app_info_h app_info, bool *enabled)
{
	bool val;

	if (app_info == NULL || enabled == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	if (pkgmgrinfo_appinfo_is_enabled(app_info->pkg_app_info, &val) < 0)
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	*enabled = val;
	return APP_MANAGER_ERROR_NONE;

}

int app_info_is_equal(app_info_h lhs, app_info_h rhs, bool *equal)
{
	if (lhs == NULL || rhs == NULL || equal == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	if (!strcmp(lhs->app_id, rhs->app_id))
	{
		*equal = true;
	}
	else
	{
		*equal = false;
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_is_onboot(app_info_h app_info, bool *onboot)
{
	bool val;

	if (app_info == NULL || onboot == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	if (pkgmgrinfo_appinfo_is_onboot(app_info->pkg_app_info, &val) < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	*onboot = val;
	return APP_MANAGER_ERROR_NONE;
}

int app_info_is_preload(app_info_h app_info, bool *preload)
{
	bool val;

	if (app_info == NULL || preload == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	if (pkgmgrinfo_appinfo_is_preload(app_info->pkg_app_info, &val) < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	*preload = val;
	return APP_MANAGER_ERROR_NONE;
}

int app_info_clone(app_info_h *clone, app_info_h app_info)
{
	int retval;

	if (clone == NULL || app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = app_info_create(app_info->app_id, clone);

	if (retval != APP_MANAGER_ERROR_NONE)
	{
		return app_manager_error(retval, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

static app_info_event_e app_info_get_app_info_event(const char *value)
{
	if (!strcasecmp(value, "install"))
	{
		return APP_INFO_EVENT_INSTALLED;
	}
	else if (!strcasecmp(value, "uninstall"))
	{
		return APP_INFO_EVENT_UNINSTALLED;	
	}
	else if (!strcasecmp(value, "update"))
	{
		return APP_INFO_EVENT_UPDATED;		
	}
	else
	{
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}
}

static int app_info_package_event_listener_cb(
	int id, const char *type, const char *package, const char *key, const char *val, const void *msg, void *data)
{
	static app_info_event_e event_type = -1;
	app_info_h app_info;

	if (!strcasecmp(key, "start"))
	{
		event_type = app_info_get_app_info_event(val);
		if (event_type >= APP_INFO_EVENT_UNINSTALLED)
		{
			if (app_info_create(package, &app_info) == APP_MANAGER_ERROR_NONE)
			{
				if(app_info_event_cb)
					app_info_event_cb(app_info, event_type, app_info_event_cb_data);

				app_info_destroy(app_info);
			}
		}
	}
	else if (!strcasecmp(key, "end") && !strcasecmp(val, "ok"))
	{
		event_type = app_info_get_app_info_event(val);
		if (event_type == APP_INFO_EVENT_INSTALLED)
		{
			if (app_info_create(package, &app_info) == APP_MANAGER_ERROR_NONE)
			{
				if(app_info_event_cb)
					app_info_event_cb(app_info, event_type, app_info_event_cb_data);

				app_info_destroy(app_info);
			}
		}
		event_type = -1;
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_set_event_cb(app_manager_app_info_event_cb callback, void *user_data)
{
	if (callback == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	if (app_info_event_cb == NULL)
	{
		package_event_listener = pkgmgr_client_new(PC_LISTENING);

		if (package_event_listener == NULL)
		{
			return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
		}
	}

	app_info_event_cb = callback;
	app_info_event_cb_data = user_data;

	pkgmgr_client_listen_status(package_event_listener, app_info_package_event_listener_cb, NULL);

	return APP_MANAGER_ERROR_NONE;
}


void app_info_unset_event_cb(void)
{
	if (app_info_event_cb != NULL)
	{
		pkgmgr_client_free(package_event_listener);
		package_event_listener = NULL;
	}

	app_info_event_cb = NULL;
	app_info_event_cb_data = NULL;
}

int app_info_filter_create(app_info_filter_h *handle)
{
	int retval = 0;
	app_info_filter_h filter_created = NULL;
	pkgmgrinfo_appinfo_filter_h filter_h = NULL;

	retval = pkgmgrinfo_appinfo_filter_create(&filter_h);
	if (retval < 0) {
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	filter_created = calloc(1, sizeof(struct app_info_filter_s));
	if (filter_created == NULL)
	{
		free(filter_h);
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	filter_created->pkg_app_info_filter = filter_h;

	*handle = filter_created;

	return APP_MANAGER_ERROR_NONE;
}

int app_info_filter_destroy(app_info_filter_h handle)
{
	int retval = 0;

	if (handle == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_filter_destroy(handle->pkg_app_info_filter);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	free(handle);
	return APP_MANAGER_ERROR_NONE;
}

int app_info_filter_add_bool(app_info_filter_h handle, const char *property, const bool value)
{
	int retval = 0;
	char *converted_property = NULL;

	if ((handle == NULL) || (property == NULL))
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = app_info_convert_bool_property(property, &converted_property);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_filter_add_bool(handle->pkg_app_info_filter, converted_property, value);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_filter_add_string(app_info_filter_h handle, const char *property, const char *value)
{
	int retval = 0;
	char *converted_property = NULL;

	if ((handle == NULL) || (property == NULL))
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = app_info_convert_str_property(property, &converted_property);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_filter_add_string(handle->pkg_app_info_filter, converted_property, value);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_filter_count_appinfo(app_info_filter_h handle, int *count)
{
	int retval = 0;

	if ((handle == NULL) || (count == NULL))
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_filter_count(handle->pkg_app_info_filter, count);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_filter_foreach_appinfo(app_info_filter_h handle, app_info_filter_cb callback, void * user_data)
{
	int retval = 0;

	foreach_context_s foreach_context = {
		.callback = callback,
		.user_data = user_data,
	};

	if ((handle == NULL) || (callback == NULL))
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_filter_foreach_appinfo(handle->pkg_app_info_filter, app_info_foreach_app_filter_cb, &foreach_context);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_metadata_filter_create(app_info_metadata_filter_h *handle)
{
	int retval = 0;
	app_info_metadata_filter_h filter_created = NULL;
	pkgmgrinfo_appinfo_metadata_filter_h filter_h = NULL;

	filter_created = calloc(1, sizeof(struct app_info_metadata_filter_s));
	if (filter_created == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_metadata_filter_create(&filter_h);
	if (retval < 0) {
		free(filter_created);
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	filter_created->pkg_app_info_metadata_filter = filter_h;

	*handle = filter_created;

	return APP_MANAGER_ERROR_NONE;
}

int app_info_metadata_filter_destroy(app_info_metadata_filter_h handle)
{
	int retval = 0;

	if (handle == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_metadata_filter_destroy(handle->pkg_app_info_metadata_filter);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	free(handle);
	return APP_MANAGER_ERROR_NONE;
}

int app_info_metadata_filter_add(app_info_metadata_filter_h handle, const char *key, const char *value)
{
	int retval = 0;

	if ((handle == NULL) || (key == NULL))
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_metadata_filter_add(handle->pkg_app_info_metadata_filter, key, value);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_info_metadata_filter_foreach(app_info_metadata_filter_h handle, app_info_filter_cb callback, void *user_data)
{
	int retval = 0;

	foreach_context_s foreach_context = {
		.callback = callback,
		.user_data = user_data,
	};

	if (callback == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	retval = pkgmgrinfo_appinfo_metadata_filter_foreach(handle->pkg_app_info_metadata_filter, app_info_foreach_app_filter_cb, &foreach_context);
	if (retval < 0)
	{
		return app_manager_error(APP_MANAGER_ERROR_IO_ERROR, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

#define __START_OF_DEPRECATED_API

static int ui_app_info_create(const char *app_id, ui_app_info_h *ui_app_info);
static int service_app_info_create(const char *app_id, service_app_info_h *service_app_info);

struct ui_app_info_s {
	char *app_id;
	pkgmgrinfo_appinfo_h ui_app_info;
};

struct service_app_info_s {
	char *app_id;
	pkgmgrinfo_appinfo_h service_app_info;
};


int ui_app_info_get_app_info(const char *app_id, ui_app_info_h *ui_app_info)
{
	return ui_app_info_create(app_id, ui_app_info);
}

int service_app_info_get_app_info(const char *app_id, service_app_info_h *service_app_info)
{
	return service_app_info_create(app_id, service_app_info);
}

static int ui_app_info_create(const char *app_id, ui_app_info_h *ui_app_info)
{
	pkgmgrinfo_appinfo_h app_info;
	ui_app_info_h info;
	if (app_id == NULL || ui_app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	if (pkgmgrinfo_appinfo_get_appinfo(app_id, &app_info)) {
		return app_manager_error(APP_MANAGER_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
	}
	info = calloc(1, sizeof(struct ui_app_info_s));
	if (info == NULL) {
		pkgmgrinfo_appinfo_destroy_appinfo(app_info);
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}
	info->app_id = strdup(app_id);
	info->ui_app_info = app_info;
	*ui_app_info = info;
	return APP_MANAGER_ERROR_NONE;
}

static int service_app_info_create(const char *app_id, service_app_info_h *service_app_info)
{
	pkgmgrinfo_appinfo_h app_info;
	service_app_info_h info;
	if (app_id == NULL || service_app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	if (pkgmgrinfo_appinfo_get_appinfo(app_id, &app_info)) {
		return app_manager_error(APP_MANAGER_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
	}
	info = calloc(1, sizeof(struct service_app_info_s));
	if (info == NULL) {
		pkgmgrinfo_appinfo_destroy_appinfo(app_info);
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}
	info->app_id = strdup(app_id);
	info->service_app_info = app_info;
	*service_app_info = info;
	return APP_MANAGER_ERROR_NONE;
}


int ui_app_info_destroy(ui_app_info_h ui_app_info)
{
	if (ui_app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	if (ui_app_info->app_id) {
		free(ui_app_info->app_id);
		ui_app_info->app_id = NULL;
	}
	pkgmgrinfo_appinfo_destroy_appinfo(ui_app_info->ui_app_info);
	free(ui_app_info);
	return APP_MANAGER_ERROR_NONE;
}

int ui_app_info_get_app_id(ui_app_info_h ui_app_info, char **app_id)
{
	char *val;
	char *app_id_dup;

	if (ui_app_info == NULL || app_id == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_appid(ui_app_info->ui_app_info, &val);
	app_id_dup = strdup(val);
	if (app_id_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*app_id = app_id_dup;
	return APP_MANAGER_ERROR_NONE;
}


int ui_app_info_get_label(ui_app_info_h ui_app_info, char **label)
{
	char *val;
	char *app_label_dup;

	if (ui_app_info == NULL || label == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_label(ui_app_info->ui_app_info, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_label_dup = strdup(val);
	if (app_label_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*label = app_label_dup;
	return APP_MANAGER_ERROR_NONE;
}

int ui_app_info_get_icon(ui_app_info_h ui_app_info, char **icon)
{
	char *val;
	char *app_icon_dup;

	if (ui_app_info == NULL || icon == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_icon(ui_app_info->ui_app_info, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_icon_dup = strdup(val);
	if (app_icon_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*icon = app_icon_dup;
	return APP_MANAGER_ERROR_NONE;
}


int ui_app_info_get_type(ui_app_info_h ui_app_info, char **type)
{
	char *val;
	char *app_type_dup;

	if (ui_app_info == NULL || type == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_apptype(ui_app_info->ui_app_info, &val);
	app_type_dup = strdup(val);
	if (app_type_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*type = app_type_dup;
	return APP_MANAGER_ERROR_NONE;
}


int ui_app_info_get_package(ui_app_info_h ui_app_info,  char **package)
{
	char *val;
	char *package_dup;

	if (ui_app_info == NULL || package == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_pkgname(ui_app_info->ui_app_info, &val);
	package_dup = strdup(val);
	if (package_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*package = package_dup;
	return APP_MANAGER_ERROR_NONE;
}


int ui_app_info_is_equal(ui_app_info_h lhs, ui_app_info_h rhs, bool *equal)
{
	if (lhs == NULL || rhs == NULL || equal == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	if (!strcmp(lhs->app_id, rhs->app_id))
	{
		*equal = true;
	}
	else
	{
		*equal = false;
	}
	return APP_MANAGER_ERROR_NONE;
}

int ui_app_info_clone(ui_app_info_h *clone, ui_app_info_h ui_app_info)
{
	int retval;
	if (clone == NULL || ui_app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	retval = ui_app_info_create(ui_app_info->app_id, clone);
	if (retval != APP_MANAGER_ERROR_NONE)
	{
		return app_manager_error(retval, __FUNCTION__, NULL);
	}
	return APP_MANAGER_ERROR_NONE;
}

int service_app_info_destroy(service_app_info_h service_app_info)
{
	if (service_app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	if (service_app_info->app_id) {
		free(service_app_info->app_id);
		service_app_info->app_id = NULL;
	}
	pkgmgrinfo_appinfo_destroy_appinfo(service_app_info->service_app_info);
	free(service_app_info);
	return APP_MANAGER_ERROR_NONE;
}

int service_app_info_get_app_id(service_app_info_h service_app_info, char **app_id)
{
	char *val;
	char *app_id_dup;

	if (service_app_info == NULL || app_id == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_appid(service_app_info->service_app_info, &val);
	app_id_dup = strdup(val);
	if (app_id_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*app_id = app_id_dup;
	return APP_MANAGER_ERROR_NONE;
}


int service_app_info_get_label(service_app_info_h service_app_info, char **label)
{
	char *val;
	char *app_label_dup;

	if (service_app_info == NULL || label == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_label(service_app_info->service_app_info, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_label_dup = strdup(val);
	if (app_label_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*label = app_label_dup;
	return APP_MANAGER_ERROR_NONE;
}

int service_app_info_get_icon(service_app_info_h service_app_info, char **icon)
{
	char *val;
	char *app_icon_dup;

	if (service_app_info == NULL || icon == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_icon(service_app_info->service_app_info, &val);
	if (val == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_icon_dup = strdup(val);
	if (app_icon_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*icon = app_icon_dup;
	return APP_MANAGER_ERROR_NONE;
}


int service_app_info_get_type(service_app_info_h service_app_info, char **type)
{
	char *val;
	char *app_type_dup;

	if (service_app_info == NULL || type == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_apptype(service_app_info->service_app_info, &val);
	app_type_dup = strdup(val);
	if (app_type_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*type = app_type_dup;
	return APP_MANAGER_ERROR_NONE;
}


int service_app_info_get_package(service_app_info_h service_app_info, char **package)
{
	char *val;
	char *package_dup;

	if (service_app_info == NULL || package == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	pkgmgrinfo_appinfo_get_pkgname(service_app_info->service_app_info, &val);
	package_dup = strdup(val);
	if (package_dup == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	*package = package_dup;
	return APP_MANAGER_ERROR_NONE;
}


int service_app_info_is_equal(service_app_info_h lhs, service_app_info_h rhs, bool *equal)
{
	if (lhs == NULL || rhs == NULL || equal == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	if (!strcmp(lhs->app_id, rhs->app_id))
	{
		*equal = true;
	}
	else
	{
		*equal = false;
	}
	return APP_MANAGER_ERROR_NONE;
}

int service_app_info_clone(service_app_info_h *clone, service_app_info_h service_app_info)
{
	int retval;
	if (clone == NULL || service_app_info == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	retval = service_app_info_create(service_app_info->app_id, clone);
	if (retval != APP_MANAGER_ERROR_NONE)
	{
		return app_manager_error(retval, __FUNCTION__, NULL);
	}
	return APP_MANAGER_ERROR_NONE;
}

int service_app_info_is_onboot(service_app_info_h service_app_info, bool *onboot)
{
	bool val;
	if (service_app_info == NULL || onboot == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	pkgmgrinfo_appinfo_is_onboot(service_app_info->service_app_info, &val);
	*onboot = val;
	return APP_MANAGER_ERROR_NONE;
}

int service_app_info_is_autorestart(service_app_info_h service_app_info, bool *autorestart)
{
	bool val;
	if (service_app_info == NULL || autorestart == NULL)
	{
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	pkgmgrinfo_appinfo_is_autorestart(service_app_info->service_app_info, &val);
	*autorestart = val;
	return APP_MANAGER_ERROR_NONE;
}

#define __END_OF_DEPRECATED_API
