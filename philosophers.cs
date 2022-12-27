using System;
using System.Threading;

namespace DiningPhilosophersProblem
{
    class Program
    {
        static void Main(string[] args)
        {
            // Create the semaphores
            Semaphore[] fork = new Semaphore[5];
            for (int i = 0; i < 5; i++)
            {
                fork[i] = new Semaphore(1, 1);
            }

            // Create the philosopher threads
            Thread[] philosophers = new Thread[5];
            for (int i = 0; i < 5; i++)
            {
                int leftfork = i;
                int rightfork = (i + 1) % 5;
                philosophers[i] = new Thread(() =>
                {
                    while (true)
                    {
                        // Think 
                        Thread.Sleep(0);

                        // Pick up the left fork
                        fork[leftfork].WaitOne();

                        // Pick up the right fork
                        fork[rightfork].WaitOne();

                        // Eat
                        Thread.Sleep(0);

                        // Put down the right fork
                        fork[rightfork].Release();

                        // Put down the left fork
                        fork[leftfork].Release();
                    }
                });
            }

            // Start the philosopher threads
            for (int i = 0; i < 5; i++)
            {
                philosophers[i].Start();
            }

            // Wait for the philosopher threads to complete
            for (int i = 0; i < 5; i++)
            {
                philosophers[i].Join();
            }
        }
    }
}
