# ConcurrentGraphs
Lock-Free Concurrent Graph using non-blocking Hashtables and BSTs

We have implemented the concurrent graph in two ways using:
1. BST for both vertex list and edge list
2. HashTables for both vertex list and edge list

## Testing-Branch

The main.cpp files in the main branch are just for checking the correctness of the code. 
For more rigorous performance testing, check the main.cpp files in the testing-branch.

It uses the RMAT graph initialization to initialize the graph.

input.txt contains the operations to be performed on the graph.

You can use dataset.cpp to generate an input.txt file according to your preference.


## Build & Run
Clone this repository and go into any of the implementation folder. For example:

```
cd BST-BST/
```
and build the main.cpp file.

```
g++ main.cpp -lpthread
```
Now, run the executable with appropriate command line arguments.

```
./a.out ARG_LIST
```
You can run the run the executable without any command line arguments to see which arguments are required.

```
./a.out
```

## Authors

* **Chander Sekhar** -  [Chander-Shekhar](https://github.com/Chander-Shekhar)
* **Bhavya Bagla** -  [bbagla](https://github.com/bbagla)


## License

This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License - see the [LICENSE.md](LICENSE.md) file for details
