# 1. DiningPhilosophers
### C# implementation of semaphore solution to the dining philosophers problem which avoids race conditions

#### Dining philosophers Problem
The Dining Philosophers Problem is a producer-consumer problem in Computer Science where you have N Philosophers siting at a table and N forks. each philosopher has one fork to their left and one fork to their right. Through traditional coding practices each philosopher would grab their left fork and then be stuck indefinitely waiting for their right fork to become free as each philosophers right fork is their right adjacent philosophers left fork. This is solved through multi-threading, semaphores, and mutexes along with allowing processes to wait their turn without busy wait.  
#### Steps:   
1. creates pthread_t threads for each philosopher 
    1. initialized as 7
2. attempt to initialize a mutex  
3. create threads for each philosopher  
    1. utilizing pthread_join to wait for each thread to be created.  
4. take_forks():  
    1. for an arbitrary number of cycles each philosopher will set their status to hungry and then take turns taking a left fork and a right fork
    2. if there is no left fork available or no right fork available, invoke pthread_cond_wait passing in the semaphore and a reference to the lock variable  
5. put_forks()
    1. when each philosopher is done eating they will set their status to thinking and then test both their adjacent philosophers to see if giving up their fork would allow them to eat. If so, invoke pthread_cond_signal passing in that philosophers semaphore to be upped.
    
# 2. Readers-Writers Problem
### C# implementation of an optimal solution to the readers writers problem which avoids race conditions
The readers-writers problem is a classic CS problem multiple threads require concurrent access to shared data. The problem is to design a solution that allows multiple "readers" to read from a shared resource simultaneously, but only allows one "writer" to write to the resource at a time
#####solution assumes that the shared resource is a simple integer variable. For more complex shared resources, a Linked List or Tree inserted inside a Data Object should be created.
