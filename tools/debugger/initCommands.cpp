/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "command.h"
#include "helpers.h"
#include "commandInterface.h"

using namespace std;

map<string, Command> sCommands;

#define COMMANDS(m) \
	m(" whatis", console_whatis)\
	m(" ptype", console_ptype)\
	m("-break-after", break_after)\
	m("-break-condition", break_condition)\
	m("-break-delete", break_delete)\
	m("-break-disable", break_disable)\
	m("-break-enable", break_enable)\
	m("-break-info", break_info)\
	m("-break-insert", break_insert)\
	m("-break-list", break_list)\
	m("-break-watch", break_watch)\
	m("-data-disassemble", data_disassemble)\
	m("-data-evaluate-expression", data_evaluate_expression)\
	m("-data-list-changed-registers", data_list_changed_registers)\
	m("-data-list-register-names", data_list_register_names)\
	m("-data-list-register-values", data_list_register_values)\
	m("-data-read-memory", data_read_memory)\
	m("-enable-timings", enable_timings)\
	m("-environment-cd", environment_cd)\
	m("-environment-directory", environment_directory)\
	m("-environment-path", environment_path)\
	m("-environment-pwd", environment_pwd)\
	m("-exec-abort", exec_abort)\
	m("-exec-arguments", exec_arguments)\
	m("-exec-continue", exec_continue)\
	m("-exec-finish", exec_finish)\
	m("-exec-interrupt", exec_interrupt)\
	m("-exec-next", exec_next)\
	m("-exec-next-instruction", exec_next_instruction)\
	m("-exec-return", exec_return)\
	m("-exec-run", exec_continue)\
	m("-exec-show-arguments", exec_show_arguments)\
	m("-exec-step", exec_step)\
	m("-exec-step-instruction", exec_step_instruction)\
	m("-exec-until", exec_until)\
	m("-file-exec-and-symbols", file_exec_and_symbols)\
	m("-file-exec-file", file_exec_file)\
	m("-file-list-exec-sections", file_list_exec_sections)\
	m("-file-list-exec-source-file", file_list_exec_source_file)\
	m("-file-list-exec-source-files", file_list_exec_source_files)\
	m("-file-list-shared-libraries", file_list_shared_libraries)\
	m("-file-list-symbol-files", file_list_symbol_files)\
	m("-file-symbol-file", file_symbol_file)\
	m("-gdb-exit", gdb_exit)\
	m("-gdb-set", gdb_set)\
	m("-gdb-show", gdb_show)\
	m("-gdb-version", gdb_version)\
	m("-inferior-tty-set", inferior_tty_set)\
	m("-inferior-tty-show", inferior_tty_show)\
	m("-interpreter-exec", interpreter_exec)\
	m("-list-features", list_features)\
	m("-stack-info-depth", stack_info_depth)\
	m("-stack-info-frame", stack_info_frame)\
	m("-stack-list-arguments", stack_list_arguments)\
	m("-stack-list-frames", stack_list_frames)\
	m("-stack-list-locals", stack_list_locals)\
	m("-stack-select-frame", stack_select_frame)\
	m("-symbol-info-address", symbol_info_address)\
	m("-symbol-info-file", symbol_info_file)\
	m("-symbol-info-function", symbol_info_function)\
	m("-symbol-info-line", symbol_info_line)\
	m("-symbol-info-symbol", symbol_info_symbol)\
	m("-symbol-list-functions", symbol_list_functions)\
	m("-symbol-list-lines", symbol_list_lines)\
	m("-symbol-list-types", symbol_list_types)\
	m("-symbol-list-variables", symbol_list_variables)\
	m("-symbol-locate", symbol_locate)\
	m("-symbol-type", symbol_type)\
	m("-target-attach", target_attach)\
	m("-target-compare-sections", target_compare_sections)\
	m("-target-detach", target_detach)\
	m("-target-disconnect", target_disconnect)\
	m("-target-download", target_download)\
	m("-target-exec-status", target_exec_status)\
	m("-target-file-delete", target_file_delete)\
	m("-target-file-get", target_file_get)\
	m("-target-file-put", target_file_put)\
	m("-target-list-available-targets", target_list_available_targets)\
	m("-target-list-current-targets", target_list_current_targets)\
	m("-target-list-parameters", target_list_parameters)\
	m("-target-select", target_select)\
	m("-thread-info", thread_info)\
	m("-thread-list-all-threads", thread_list_all_threads)\
	m("-thread-list-ids", thread_list_ids)\
	m("-thread-select", thread_select)\
	m("-var-assign", var_assign)\
	m("-var-create", var_create)\
	m("-var-delete", var_delete)\
	m("-var-evaluate-expression", var_evaluate_expression)\
	m("-var-info-expression", var_info_expression)\
	m("-var-info-num-children", var_info_num_children)\
	m("-var-info-path-expression", var_info_path_expression)\
	m("-var-info-type", var_info_type)\
	m("-var-list-children", var_list_children)\
	m("-var-set-format", var_set_format)\
	m("-var-set-frozen", var_set_frozen)\
	m("-var-show-attributes", var_show_attributes)\
	m("-var-show-format", var_show_format)\
	m("-var-update", var_update)\
	m("-test-wait", test_wait)\

#define DECLARE_FUNCTION(cmd, func) void func(const string&);
COMMANDS(DECLARE_FUNCTION);

void initCommands() {
#define INSERT(cmd, func) sCommands.insert(CommandPair(cmd, func));
	COMMANDS(INSERT);
}


#define UNIMPL error("Unimplemented MI command: %s", __FUNCTION__)

void break_after(const string& args) {
	UNIMPL;
}
void break_condition(const string& args) {
	UNIMPL;
}
void break_info(const string& args) {
	UNIMPL;
}
void break_watch(const string& args) {
	UNIMPL;
}
void enable_timings(const string& args) {
	UNIMPL;
}
void environment_cd(const string& args) {
	UNIMPL;
}
void environment_directory(const string& args) {
	UNIMPL;
}
void environment_path(const string& args) {
	UNIMPL;
}
void environment_pwd(const string& args) {
	UNIMPL;
}
void exec_abort(const string& args) {
	UNIMPL;
}
void exec_arguments(const string& args) {
	UNIMPL;
}

void exec_finish(const string& args) {
	UNIMPL;
}
void exec_return(const string& args) {
	UNIMPL;
}
void exec_show_arguments(const string& args) {
	UNIMPL;
}
void exec_until(const string& args) {
	UNIMPL;
}
void file_exec_file(const string& args) {
	UNIMPL;
}
void file_list_exec_sections(const string& args) {
	UNIMPL;
}
void file_list_exec_source_file(const string& args) {
	UNIMPL;
}
void file_list_shared_libraries(const string& args) {
	UNIMPL;
}
void file_list_symbol_files(const string& args) {
	UNIMPL;
}
void file_symbol_file(const string& args) {
	UNIMPL;
}
void gdb_show(const string& args) {
	UNIMPL;
}
void inferior_tty_set(const string& args) {
	UNIMPL;
}
void inferior_tty_show(const string& args) {
	UNIMPL;
}
void list_features(const string& args) {
	UNIMPL;
}
void symbol_info_address(const string& args) {
	UNIMPL;
}
void symbol_info_file(const string& args) {
	UNIMPL;
}
void symbol_info_function(const string& args) {
	UNIMPL;
}
void symbol_info_line(const string& args) {
	UNIMPL;
}
void symbol_info_symbol(const string& args) {
	UNIMPL;
}
void symbol_list_functions(const string& args) {
	UNIMPL;
}
void symbol_list_lines(const string& args) {
	UNIMPL;
}
void symbol_list_types(const string& args) {
	UNIMPL;
}
void symbol_list_variables(const string& args) {
	UNIMPL;
}
void symbol_locate(const string& args) {
	UNIMPL;
}
void symbol_type(const string& args) {
	UNIMPL;
}
void target_attach(const string& args) {
	UNIMPL;
}
void target_compare_sections(const string& args) {
	UNIMPL;
}
void target_detach(const string& args) {
	UNIMPL;
}
void target_disconnect(const string& args) {
	UNIMPL;
}
void target_download(const string& args) {
	UNIMPL;
}
void target_exec_status(const string& args) {
	UNIMPL;
}
void target_file_delete(const string& args) {
	UNIMPL;
}
void target_file_get(const string& args) {
	UNIMPL;
}
void target_file_put(const string& args) {
	UNIMPL;
}
void target_list_available_targets(const string& args) {
	UNIMPL;
}
void target_list_current_targets(const string& args) {
	UNIMPL;
}
void target_list_parameters(const string& args) {
	UNIMPL;
}
void thread_list_all_threads(const string& args) {
	UNIMPL;
}
void thread_list_ids(const string& args) {
	UNIMPL;
}
void thread_select(const string& args) {
	UNIMPL;
}
void var_assign(const string& args) {
	UNIMPL;
}
void var_set_frozen(const string& args) {
	UNIMPL;
}
