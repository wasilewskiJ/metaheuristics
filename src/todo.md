 ---                                                          
  What's done correctly:                                       
  - File loader (load_from_file) ✅                            
  - calculate_total_time — formula matches the instruction's
  c(xj, m) definition ✅                                       
  - randomAlg / runRandomAlg ✅                                
  - Solution struct ✅         
  - Debug infrastructure ✅                                    
  - Exception handling in main.cpp ✅
                                                               
  ---                                                          
  Missing (required for Lab 1):
  - Greedy algorithm — the instruction explicitly requires it  
  for Lab 1 ("Algorytm zachłanny -- implementacja"). It should
  build a solution by always picking the job with the smallest 
  c(xj, M) contribution at each step.
                                                               
  ---             
  Bugs / issues found:

  Location: PfspInstance.cpp:104                               
  Issue: time_spent_on_machine array is declared and written   
  but    never read — dead code.
  ────────────────────────────────────────                     
  Location: logger.hpp:9                                       
  Issue: dumpToFile() is an empty stub — the instruction
    requires CSV logging (<nr_pokolenia; najlepsza_ocena;      
    srednia_ocen; najgorsza_ocena;;;>)
                                                               
  ---             
  What comes next (Lab 2):
  - Mutation operators (swap, inversion)
  - Crossover operators (OX, PMX, or CX)
  - Selection (tournament / roulette)   
  - EA population management                                   
  - Another metaheuristic (Tabu Search or Simulated Annealing) 
  - CSV logger for charts (best/avg/worst per generation)      
                                                               
  The biggest gap right now is the greedy algorithm —          
  everything else in Lab 1 is present, just with the quality   
  issues listed above.         


*    temp: REMOVE LATER! notes in EA.cpp 
