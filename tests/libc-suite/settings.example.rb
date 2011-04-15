SETTINGS = {
	:source_path => '/glibc-2.11.1/',
	:stop_on_fail => true,
	:rebuild_failed => true,
	:retry_failed => true,
	:rebuild_missing_log => true,
	:test_dead_code_elimination => false,
	:test_release => false,
	:copy_targets => [],
	:loader_base_url => 'http://localhost:5002/',
	:htdocs_dir => '/htdocs/',
}
