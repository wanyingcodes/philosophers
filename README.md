# 🍝 Philosophers

## 🧠 Project Overview

This project is an implementation of the classic **Dining Philosophers Problem**, a well-known concurrency challenge in computer science.  
The simulation models philosophers sitting around a table, alternating between **eating**, **sleeping**, and **thinking**, while competing for limited shared resources (forks).  
The goal is to manage thread synchronization properly to **avoid deadlocks, data races, and starvation**.  
Each philosopher is represented as a thread, and forks are protected using mutexes to ensure safe access.  

## ✨ Features

* 🧵 Multi-threaded simulation using `pthread`  
* 🔒 Mutex-based fork protection (no duplication of resources)  
* ⏱ Precise time management using `gettimeofday`  
* 💀 Death detection within 10ms constraint  
* 🚫 No data races (thread-safe implementation)  
* 🔁 Optional meal limit to stop simulation  
* 📜 Clean and synchronized logging output  

## 🚀 Installation & Usage

### 1. Clone the repository

```bash
git clone git@github.com:wanyingcodes/philosophers.git
cd philosophers/philo
```

### 2. Compile

```bash
make
```

### 3. Run

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Example

```bash
./philo 5 800 200 200
```

## 📜 Rules & Constraints

The simulation follows strict rules defined by the subject:  

### 🧠 General Behavior

* Philosophers sit around a circular table with one fork between each pair  
* Each philosopher alternates between:  

  * **eating**  
  * **sleeping**  
  * **thinking**  
* To eat, a philosopher must hold **both left and right forks**  
* After eating, forks are released and the philosopher sleeps, then thinks  
* The simulation stops when:  

  * a philosopher **dies**, or  
  * *(optional)* all philosophers have eaten a required number of meals  

### ⏱ Timing Constraints

* Each philosopher must eat before `time_to_die` expires  
* If not, they **die of starvation**  
* State changes must be logged with a timestamp in milliseconds  
* A death message must be printed **within 10 ms** of actual death  

### 🧵 Concurrency Requirements

* Each philosopher is implemented as a **separate thread**  
* Forks are shared resources protected by **mutexes**  
* The program must:  

  * ❌ Avoid **data races**  
  * ❌ Avoid **deadlocks**  
  * ❌ Prevent **fork duplication**  

### 📥 Program Arguments

```bash
number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### 📢 Logging Format

Each state change must follow this exact format:  

```text
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```

* Logs must not overlap  
* Output must remain synchronized and readable  

### ⚠️ Additional Constraints

* 🚫 No global variables allowed  
* 🧪 Must comply with allowed functions only (`pthread`, `usleep`, etc.)  
* 🧼 Clean memory management required  

## 🧪 Testing

The following tests are based on the official subject requirements and are used to validate correctness, timing precision, and synchronization.  

### ⚠️ Testing Constraints

* Do not test with more than **200 philosophers**  
* Do not use values lower than **60 ms** for:  

  * `time_to_die`  
  * `time_to_eat`  
  * `time_to_sleep`  

### ✅ Mandatory Test Cases

#### 🔴 Death cases

```bash
./philo 1 800 200 200
```

* The philosopher should **not be able to eat**  
* The philosopher **must die**  

```bash
./philo 4 310 200 100
```

* **One philosopher should die**  

#### 🟢 Survival cases

```bash
./philo 5 800 200 200
```

* **No philosopher should die**  

```bash
./philo 4 410 200 200
```

* **No philosopher should die**  

#### 🔁 Meal limit test

```bash
./philo 5 800 200 200 7
```

* No philosopher should die  
* Simulation must stop when **each philosopher has eaten at least 7 times**  

### ⏱ Timing Accuracy Test

```bash
./philo 2 800 200 200
```

* A philosopher’s death must be logged **within 10 ms**  
* Any delay greater than 10 ms is considered **incorrect**  

### 🔍 Additional Validation

Test with custom values to ensure:  

* ⏳ Philosophers **die at the correct time**  
* 🍝 No philosopher eats without holding **two forks**  
* 🍴 Forks are not shared incorrectly (no duplication)  
* 🧵 No data races or undefined behavior  
* 🧾 Logs are **synchronized and non-overlapping**  
* 🚫 No deadlocks (simulation should not freeze)  

## 📁 Project Structure

```text
philo/
├── Makefile
├── philo.c            # entry point
├── init.c             # initialization and setup
├── routine.c          # philosopher behavior loop
├── routine_utils.c    # helper functions for routines
├── monitor.c          # death detection and simulation control
├── utils.c            # time and utility functions
├── philo.h            # shared structures and prototypes
```

## 💡 Design Decisions & Key Learnings

During this project, the main challenge was not just implementing concurrency, but understanding what correctness and fairness actually mean in a real multi-threaded system.  

Several initial assumptions turned out to be misleading, and the final design reflects a series of deliberate trade-offs rather than textbook-perfect solutions.  

### 🧵 1. Global Start Time: Fairness vs Reality

Initially I attempted to synchronize philosophers’ start using usleep, start barriers, or post‑creation delays—but execution order is not guaranteed in concurrency. A bug emerged: the monitor thread ran before philosophers initialized last_meal_time, causing immediate death.  

Solution: Redefined start time as a logical reference, not physical. Record a single timestamp before any thread creation; all philosophers initialize last_meal_time to that value; timing uses now - start_time. This eliminates uninitialized states, ensures consistent timing, and avoids reliance on OS scheduling.  

### 🚫 2. Why “Perfect Synchronization” Was Rejected

A true barrier would ensure simultaneous start, but it's disallowed by project constraints. Implementing a custom barrier adds complexity, and the timing tolerance (≤10 ms) already accepts small offsets. More importantly, perfect synchronization is unnecessary and can be harmful—it spikes contention and increases starvation risk. Small startup offsets are acceptable and actually improve stability.  

### ⏱ 3. Responsiveness Over Blocking Design

Initially, stop conditions were checked only at loop boundaries, causing long uninterruptible usleep delays and slow death reactions. I refactored to check conditions frequently and replaced long sleeps with short intervals (usleep(100) loops). This ensures fast death detection and maintains correctness under tight timing constraints.  

### 💀 4. Death Detection & Global Stop Propagation

The project required that philosophers cannot directly know others’ states, yet the simulation must stop globally when death occurs. I used a dedicated monitor thread to continuously check for starvation and set a shared stop_simulation flag. All other threads periodically check this flag and exit gracefully. This decouples detection from execution and guarantees consistent shutdown.  

### 🍝 5. Meal Completion vs Death: Two Stop Semantics

I handled two termination conditions differently: reaching the meal limit triggers natural exit, while death requires an immediate global stop. To avoid conflicts, I protected shared state with mutexes and carefully prevented simultaneous “death” and “completion” outputs—ensuring consistent, duplicate-free logs.  

### 🔒 6. Synchronization Strategy (Minimal but Sufficient)

I used mutexes only where strictly necessary: forks to prevent concurrent access, meal state to protect last_meal_time and counters, death state to avoid race conditions on termination, and a print lock for clean output. I prevented deadlock via resource ordering—achieving no data races, no deadlocks, and minimal locking complexity.  

### ⚖️ 7. Starvation Mitigation: Controlled Imperfection

Deadlock-free doesn’t guarantee starvation-free—pure ordering can still leave some philosophers never eating. I introduced **two layers of controlled variation**: first, staggered startup with odd/even delays plus small per‑philosopher perturbations; second, adaptive thinking times—hungrier philosophers think less, and boundary philosophers (IDs 1 and N) get shorter pauses to break symmetry. This reduces contention peaks, improves fairness in practice, and stays within project constraints.  

### ⚡ 8. Monitoring Frequency vs CPU Usage

To meet the ≤10 ms death detection constraint, I made a key trade‑off: the monitor loop does not sleep at all. Why? Even a 100 ms sleep would risk exceeding the 10 ms error margin, because after usleep returns, I cannot control how soon the OS schedules the thread again. Only continuous polling guarantees precise timing. This sacrifices CPU efficiency to ensure correctness under the project’s hard deadline.  

### 🧠 Key Takeaways

- Time in concurrency is a shared reference, not a synchronized event
- OS scheduling cannot be controlled — design must adapt to it
- “Fairness” means consistent rules, not identical execution timing
- Overengineering (barriers, perfect sync) often hurts more than helps
- Small imperfections like timing offsets can improve real system behavior
- A robust solution balances correctness, simplicity, and constraints

## 📬 Contact

GitHub: https://github.com/wanyingcodes  
Email: wanyingt@hotmail.com
