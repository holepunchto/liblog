## liblog

A simple logging library with a unified interface to [os_log](https://developer.apple.com/documentation/os/logging?language=objc), [syslog](https://www.man7.org/linux/man-pages/man3/syslog.3.html), and [TraceLogging](https://learn.microsoft.com/en-us/windows/win32/tracelogging/).

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

On Linux, [journalctl](https://www.man7.org/linux/man-pages/man1/journalctl.1.html) can be used to consume logs. To show only messages for a given log, such as `some-log` in the previous example, pass the `--identifier` flag:

```sh
journalctl --identifier <name>
```

#### Windows

On Windows, liblog registers a TraceLogging provider with the following information: 

- Name: `liblog`
- GUID: `e51b91fb-bf54-53ca-bc02-d02207269772` (hash of `liblog`)

The [`tracelog`](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/tracelog) and [`traceview`](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/traceview) tools from the [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) can be used to consume logs.

## API

See [`includes/log.h`](include/log.h) for the public API.

## License

ISC
