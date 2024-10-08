## liblog

Simple logging library with a unified interface to [os_log](https://developer.apple.com/documentation/os/logging?language=objc), [syslog](https://www.man7.org/linux/man-pages/man3/syslog.3.html), [Logcat](https://developer.android.com/tools/logcat), and [TraceLogging](https://learn.microsoft.com/en-us/windows/win32/tracelogging/).

## Usage

```c
#include <log.h>

log_open("some-log", 0); 

log_debug("Current pid: %u", getpid());

log_close();
```

### Consuming logs

#### macOS

On macOS, [Console.app](https://support.apple.com/en-gb/guide/console/welcome/mac) and the `log` tool can be used to consume logs. To show only message for a given log, such as `some-log` in the previous example, pass the `--predicate` flag:

```sh
log stream --predicate "subsystem == '<name>'"
```

#### Linux

On Linux, [`journalctl`](https://www.man7.org/linux/man-pages/man1/journalctl.1.html) can be used to consume logs. To show only messages for a given log, such as `some-log` in the previous example, pass the `--identifier` flag:

```sh
journalctl --identifier <name>
```

#### Android

On Android, [`logcat`](https://developer.android.com/tools/logcat) can be used to consume logs. To show only messages for a given log, such as `some-log` in the previous example, pass a filterspec:

```sh
adb logcat "*:S some-log:*"
```

This will silence all logs by default (`*:S`) and show only those with a name of `some-log` for all priorities (`:*`).

#### Windows

On Windows, the [`tracelog`](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/tracelog) and [`traceview`](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/traceview) tools from the [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) can be used to consume logs. To start a trace on the command line using `tracelog` for a given log, such as `some-log` in the previous example, do:

```pwsh
tracelog -start my-trace -guid *some-log -f log.etl
```

When done collecting logs, stop the trace:

```pwsh
tracelog -stop my-trace
```

To summarize and view the collected logs, use [`tracefmt`](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/tracefmt):

```pwsh
tracefmt log.etl -o log.txt
```

## API

See [`include/log.h`](include/log.h) for the public API.

## License

Apache 2.0
