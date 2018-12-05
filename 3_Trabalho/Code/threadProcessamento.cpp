// =======================================================================
// thread para processamento dos dados na struct
// =======================================================================
#include "threadProcessamento.h"

void* procDados(void* i){
  // Calculo do ângulo de Flexão
  anguloDeFlexao.meiaAsaDireita = copysign((imu_struct[(int*) i].pitch - std::abs(imu_struct[2].pitch)), imu_struct[(int*) i].pitch);
  // Calculo do ângulo de Torção
  anguloDeTorcao.meiaAsaDireita = copysign((imu_struct[(int*) i].roll - std::abs(imu_struct[2].roll)), imu_struct[(int*) i].roll);

  return NULL;
} // FIM DA THREAD processamentoDados
