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
	m(" whatis", console_whatis, 0)\
	m(" ptype", console_ptype, 0)\
	m("-break-after", break_after, 0)\
	m("-break-condition", break_condition, 0)\
	m("-break-delete", break_delete, 0)\
	m("-break-disable", break_disable, 0)\
	m("-break-enable", break_enable, 0)\
	m("-break-info", break_info, 0)\
	m("-break-insert", break_insert, 0)\
	m("-break-list", break_list, 0)\
	m("-break-watch", break_watch, 0)\
	m("-data-disassemble", data_disassemble, 0)\
	m("-data-evaluate-expression", data_evaluate_expression, 0)\
	m("-data-list-changed-registers", data_list_changed_registers, 0)\
	m("-data-list-register-names", data_list_register_names, 0)\
	m("-data-list-register-values", data_list_register_values, 0)\
	m("-data-read-memory", data_read_memory, 0)\
	m("-enable-timings", enable_timings, 0)\
	m("-environment-cd", environment_cd, 0)\
	m("-environment-directory", environment_directory, 0)\
	m("-environment-path", environment_path, 0)\
	m("-environment-pwd", environment_pwd, 0)\
	m("-exec-abort", exec_abort, 0)\
	m("-exec-arguments", exec_arguments, 0)\
	m("-exec-continue", exec_continue, 0)\
	m("-exec-finish", exec_finish, 0)\
	m("-exec-interrupt", exec_interrupt, 0)\
	m("-exec-next", exec_next, 0)\
	m("-exec-next-instruction", exec_next_instruction, 0)\
	m("-exec-return", exec_return, 0)\
	m("-exec-run", exec_continue, 0)\
	m("-exec-show-arguments", exec_show_arguments, 0)\
	m("-exec-step", exec_step, 0)\
	m("-exec-step-instruction", exec_step_instruction, 0)\
	m("-exec-until", exec_until, 0)\
	m("-file-exec-and-symbols", file_exec_and_symbols, 0)\
	m("-file-exec-file", file_exec_file, 0)\
	m("-file-list-exec-sections", file_list_exec_sections, 0)\
	m("-file-list-exec-source-file", file_list_exec_source_file, 0)\
	m("-file-list-exec-source-files", file_list_exec_source_files, 0)\
	m("-file-list-shared-libraries", file_list_shared_libraries, 0)\
	m("-file-list-symbol-files", file_list_symbol_files, 0)\
	m("-file-symbol-file", file_symbol_file, 0)\
	m("-gdb-exit", gdb_exit, 0)\
	m("-gdb-set", gdb_set, 0)\
	m("-gdb-show", gdb_show, 0)\
	m("-gdb-version", gdb_version, 0)\
	m("-inferior-tty-set", inferior_tty_set, 0)\
	m("-inferior-tty-show", inferior_tty_show, 0)\
	m("-interpreter-exec", interpreter_exec, 0)\
	m("-list-features", list_features, 0)\
	m("-stack-info-depth", stack_info_depth, 0)\
	m("-stack-info-frame", stack_info_frame, 0)\
	m("-stack-list-arguments", stack_list_arguments, 0)\
	m("-stack-list-frames", stack_list_frames, 0)\
	m("-stack-list-locals", stack_list_locals, 0)\
	m("-stack-select-frame", stack_select_frame, 0)\
	m("-symbol-info-address", symbol_info_address, 0)\
	m("-symbol-info-file", symbol_info_file, 0)\
	m("-symbol-info-function", symbol_info_function, 0)\
	m("-symbol-info-line", symbol_info_line, 0)\
	m("-symbol-info-symbol", symbol_info_symbol, 0)\
	m("-symbol-list-functions", symbol_list_functions, 0)\
	m("-symbol-list-lines", symbol_list_lines, 0)\
	m("-symbol-list-types", symbol_list_types, 0)\
	m("-symbol-list-variables", symbol_list_variables, 0)\
	m("-symbol-locate", symbol_locate, 0)\
	m("-symbol-type", symbol_type, 0)\
	m("-target-attach", target_attach, 0)\
	m("-target-compare-sections", target_compare_sections, 0)\
	m("-target-detach", target_detach, 0)\
	m("-target-disconnect", target_disconnect, 0)\
	m("-target-download", target_download, 0)\
	m("-target-exec-status", target_exec_status, 0)\
	m("-target-file-delete", target_file_delete, 0)\
	m("-target-file-get", target_file_get, 0)\
	m("-target-file-put", target_file_put, 0)\
	m("-target-list-available-targets", target_list_available_targets, 0)\
	m("-target-list-current-targets", target_list_current_targets, 0)\
	m("-target-list-parameters", target_list_parameters, 0)\
	m("-target-select", target_select, 0)\
	m("-thread-info", thread_info, 0)\
	m("-thread-list-all-threads", thread_list_all_threads, 0)\
	m("-thread-list-ids", thread_list_ids, 0)\
	m("-thread-select", thread_select, 0)\
	m("-var-assign", var_assign, 0)\
	m("-var-create", var_create, Command::LOAD_STACK)\
	m("-var-delete", var_delete, 0)\
	m("-var-evaluate-expression", var_evaluate_expression, Command::LOAD_STACK)\
	m("-var-info-expression", var_info_expression, 0)\
	m("-var-info-num-children", var_info_num_children, 0)\
	m("-var-info-path-expression", var_info_path_expression, 0)\
	m("-var-info-type", var_info_type, 0)\
	m("-var-list-children", var_list_children, 0)\
	m("-var-set-format", var_set_format, 0)\
	m("-var-set-frozen", var_set_frozen, 0)\
	m("-var-show-attributes", var_show_attributes, 0)\
	m("-var-show-format", var_show_format, 0)\
	m("-var-update", var_update, Command::LOAD_STACK)\
	m("-test-wait", test_wait, 0)\

#define DECLARE_FUNCTION(cmd, func, options) void func(const string&);
COMMANDS(DECLARE_FUNCTION);

void initCommands() {
	#define INSERT(cmd, func, options) sCommands.insert(CommandPair(cmd, Command(func, options)));
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
void exec_return(const string& args) {
	UNIMPL;
}
void exec_show_arguments(const string& args) {
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
