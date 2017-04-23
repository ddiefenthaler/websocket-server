## Thread Structure


### First approach

- todo appropiate term for package -

The program consists of four different type of threads.
The number of threads per type varies depending on in which mode it is running. - todo ref earlier section
The following table lists the threads per mode.

| Server mode | Client mode |
| ----------- | ----------- |
| 1× select   |             |
| 2× receive  | 1× receive  |
| 4× handle   | 1× handle   |
| 1× observer | 1× observer |

- todo system overview with thread types marked in it -

The communication between the threads happens through simple queues.

Select checks for incoming packages on the open sockets.
This is done using the system call select - todo ref mangpage.
Another possibility would be the use of libevent.
All sockets with incoming packages get written into a queue to be read by the receive threads.
Perhaps there is also some kind of back channel, so that select only listens on the the previously - empty (todo better word) - sockets.
As for a client only one connection is open this thread is not needed.

The receive threads read the data from the sockets.
Before putting it into the next queue it also decodes the packages - ref proxy protection.
Maybe it also waits until the complete package has arrived handling fragmentation this way.
This is subject to change if reaction to the fragments is desired before the whole packages is sent (streams perhaps),

Handle threads take packages from the queue and process them in a way similar to pattern matching.
If possible processing of user packages is done through dynamic loaded librarys - todo linux term.
As there are more threads of this type, threads are allowed to take longer to deal with the packages.
But they shouldn't block waiting for other packages to prevent deadlocks.
Fine granular priority level should ensure quick handling of control frames nevertheless (Ping request for example).

The observer is a control thread that can read from a pipe/stdin to trigger actions.
It should also be able to trigger ping request on its own if requested in a configuration file.
A possibility is that relaunching the programm from the command line with extra parameters will pass them to the observer.
This would enable use cases like this: `./creative-program-name broadcast Hello world!`

Even though for a client one thread could be enough this structure will stay mostly in place to allow the quicker handling of control frames.


### Further considerations

After a conversation with the advisor of this practicum ...
comparison task based models


### Final thread model


### Excursion: Pritrity queue



