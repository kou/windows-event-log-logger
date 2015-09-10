#include <windows.h>

int
main(int argc, char **argv)
{
  const char *event_source_name = "MyApp";
  HANDLE event_source;
  WORD type = EVENTLOG_ERROR_TYPE;
  WORD category = 0;
  DWORD event_id = 0;
  SID *user_sid = NULL;
  WORD n_strings = 1;
  DWORD data_size = 0;
  const char *strings[] = {"File not found"};
  void *data = NULL;

  event_source = RegisterEventSource(NULL, event_source_name);
  ReportEvent(event_source, type, category, event_id, user_sid,
              n_strings, data_size,
              strings, data);
  DeregisterEventSource(event_source);

  return 0;
}
