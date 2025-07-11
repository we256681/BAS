{
    var thread_data = _thread_get_status(<%= thread_id %>)
    <%= result %> = thread_data.result
    <%= is_running %> = thread_data.is_running
    <%= error %> = thread_data.error
    <%= is_success %> = thread_data.is_success
}
