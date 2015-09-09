#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int
main(int argc, char **argv)
{
  const char *event_source_name;
  HANDLE event_source;
  const char *type_name;
  WORD type;
  WORD category = 0;
  DWORD event_id = 0;
  TOKEN_USER *user;
  SID *user_sid = NULL;
  WORD n_strings = 1;
  DWORD data_size = 0;
  const char *strings[1];
  void *data = NULL;

  if (argc != 4) {
    fprintf(stderr,
            "Usage: %s SOURCE_NAME info|warn|error MESSAGE\n",
            argv[0]);
    fprintf(stderr,
            " e.g.: %s MyApp error 'This is an error message'\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  event_source_name = argv[1];
  event_source = RegisterEventSourceA(NULL, event_source_name);
  if (event_source == INVALID_HANDLE_VALUE) {
    fprintf(stderr,
            "Failed to register event source: <%s>: <%lu>\n",
            event_source_name,
            GetLastError());
    return EXIT_FAILURE;
  }

  type_name = argv[2];
  if (strcmp(type_name, "info") == 0) {
    type = EVENTLOG_INFORMATION_TYPE;
  } else if (strcmp(type_name, "warn") == 0) {
    type = EVENTLOG_WARNING_TYPE;
  } else if (strcmp(type_name, "error") == 0) {
    type = EVENTLOG_ERROR_TYPE;
  } else {
    DeregisterEventSource(event_source);
    fprintf(stderr,
            "Event type must be info, warn or error: <%s>\n",
            type_name);
    return EXIT_FAILURE;
  }

  {
    HANDLE process;
    HANDLE token;
    DWORD user_size;

    process = GetCurrentProcess();
    OpenProcessToken(process, TOKEN_READ, &token);
    GetTokenInformation(token, TokenUser, NULL, 0, &user_size);
    user = malloc(user_size);
    GetTokenInformation(token, TokenUser, user, user_size, &user_size);
    user_sid = user->User.Sid;
    CloseHandle(token);
  }

  strings[0] = argv[3];
  ReportEventA(event_source, type, category, event_id, user_sid,
               n_strings, data_size,
               strings, data);

  free(user);

  DeregisterEventSource(event_source);

  return EXIT_SUCCESS;
}
