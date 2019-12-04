#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
  // aggregate processor information
  int init_user_, init_nice_, init_system_, init_idle_, init_iowait_,
   init_irq_, init_softirq_, init_steal_, init_guest_, init_guest_nice_;
  int total_init_idle_{0}, total_init_nonidle_{0};
};

#endif