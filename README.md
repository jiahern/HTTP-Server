# HTTP-Server
Building HTTP Server that responds correctly to a limited set of GET requests. 
The HTTP server must return valid response headers for a range of files and paths. 

## Summary
•	Understood HTTP 1.0 Protocol.\
•	Practiced with socket programming.\
•	Configured with IPv4 and IPv6.\
•	Respond to client's GET requests for static files.\
•	Constructed parallel downloads and multi-threads functionality by pthread

## Task & Requirements

### 1.Server runs and sends valid responses
Code must run on the marking VM without crashing (e.g. seg faulting) regardless of the inputs. Any code that crashes for any reason may be allocated a score of 0 overall (not just for this component). However, if your code crashes at the submission deadline, still submit it because some marks may be awarded.\
\
Server sends a valid HTTP 200 response in reply to a GET request for an HTML file located in web root directory (not a sub-directory)\
\
Server sends a valid HTTP 404 response in reply to a GET request for a file in the web root directory that does not exist or cannot be opened for reading\
\
(If you are a purist, then if the file exists but cannot be opened for reading, then the program can alternatively return a 403 (Forbidden Error). No extra marks are allocated for that.)\
\
GET requests using path components ../ should return a 404 error. (Otherwise a query like ../secret would break out of the web root, unless handled specifically.)

### 2.Server MIME Types and Paths
Server sends a valid HTTP 200 response with the correct MIME type in reply to a GET request for a file located in web root directory.\
\
Server sends a valid HTTP 200 response with the correct MIME type in reply to a GET request with a path below the web root for any of the specified file types (e.g. GET /css/style.css HTTP/1.0).

### 3.IPv6
The server can accept connections and perform Tasks 1 and 2 using IPv6.

### 4.Parallel downloads
Server uses Pthreads (or similar concurrent programming technique, or epoll) to process incoming requests and sending responses. Do not fork multiple processes. Server can process and respond to at least five HTTP requests concurrently. There is no need to limit this to only five.

_more details in project specification!_
