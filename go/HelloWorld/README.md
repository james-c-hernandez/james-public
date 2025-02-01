# My Go Project

This project is a simple Go application that outputs "Hello World from Go!" to the console.

## Getting Started

To run this project, you need to have Go installed on your machine. Follow the instructions below to get started.

### Prerequisites

- Go (version 1.16 or later)

### Running the Application

1. Clone the repository or download the project files.
2. Navigate to the project directory:
   ```
   cd project-laypout
   ```
3. To compile the application: 
   ```
   cd src
   go build main.go goodbye.go
   then 
   ./main.exe
   ```
3. or Run the application:
   ```
   go run main.go googbye.go
   ```

You should see the output:
```
Hello World from Go!
```


Note: this uses an external package that was loaded from the command line from the folder helloworld, that automatically updates the go.mod and go.sum files:
go get github.com/enescakir/emoji 
