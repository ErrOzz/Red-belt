#include <iostream>
#include <queue>
#include <set>

using namespace std;

struct scheduler_t{
  queue<int> threads_queue;
  set<int> blocked_threads;
  int current_time;
  int timeslice;
} scheduler;

/**
 * Функция будет вызвана перед каждым тестом, если вы
 * используете глобальные и/или статические переменные
 * не полагайтесь на то, что они заполнены 0 - в них
 * могут храниться значения оставшиеся от предыдущих
 * тестов.
 *
 * timeslice - квант времени, который нужно использовать.
 * Поток смещается с CPU, если пока он занимал CPU функция
 * timer_tick была вызвана timeslice раз.
 **/
void scheduler_setup(int timeslice) {
  scheduler = {queue<int>(), set<int>(), 0, timeslice};
}

/**
 * Функция вызывается, когда создается новый поток управления.
 * thread_id - идентификатор этого потока и гарантируется, что
 * никакие два потока не могут иметь одинаковый идентификатор.
 **/
void new_thread(int thread_id) {
  if (!scheduler.threads_queue.size()) scheduler.current_time = 0;
  scheduler.threads_queue.push(thread_id);
}

/**
 * Функция вызывается, когда поток, исполняющийся на CPU,
 * завершается. Завершится может только поток, который сейчас
 * исполняется, поэтому thread_id не передается. CPU должен
 * быть отдан другому потоку, если есть активный
 * (незаблокированный и незавершившийся) поток.
 **/
void exit_thread() {
  scheduler.current_time = 0;
  scheduler.threads_queue.pop();
}

/**
 * Функция должна возвращать идентификатор потока, который в
 * данный момент занимает CPU, или -1 если такого потока нет.
 * Единственная ситуация, когда функция может вернуть -1, это
 * когда нет ни одного активного потока (все созданные потоки
 * либо уже завершены, либо заблокированы).
 **/
int current_thread() {
  return scheduler.threads_queue.size() ? scheduler.threads_queue.front() : -1;
}

/**
 * Функция вызывается, когда поток, исполняющийся на CPU,
 * блокируется. Заблокироваться может только поток, который
 * сейчас исполняется, поэтому thread_id не передается. CPU
 * должен быть отдан другому активному потоку, если таковой
 * имеется.
 **/
void block_thread() {
  if (current_thread() == -1) return;
  scheduler.blocked_threads.insert(scheduler.threads_queue.front());
  exit_thread();
}

/**
 * Функция вызывается, когда один из заблокированных потоков
 * разблокируется. Гарантируется, что thread_id - идентификатор
 * ранее заблокированного потока.
 **/
void wake_thread(int thread_id) {
  new_thread(thread_id);
  scheduler.blocked_threads.erase(thread_id);
}

/**
 * Ваш таймер. Вызывается каждый раз, когда проходит единица
 * времени.
 **/
void timer_tick() {
  ++scheduler.current_time;
  if (scheduler.current_time == scheduler.timeslice &&
      scheduler.threads_queue.size()) {
    new_thread(scheduler.threads_queue.front());
    exit_thread();
  }
}

int main() {
  scheduler_setup(4);

    new_thread(1);
    new_thread(2);
    new_thread(3);

    timer_tick();
    timer_tick();

    exit_thread();

    cout << "current thread: " << current_thread() << endl;

    for (int i = 0; i < 14; i++){
      timer_tick();
      cout << "current thread: " << current_thread() << endl;
    }
}
