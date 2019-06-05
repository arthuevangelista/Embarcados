# ======================================
# SISTEMA DE AQUISIÇÃO DE DADOS
# ======================================
  Universidade de Brasília
  campus Gama
 
  Versão: rev 2.0
  Autor: Arthur Evangelista
  Matrícula: 14/0016686
 
  Código open-source
  =======================================================================
  Este código implementa um sistema de aquisição de dados. Ele utiliza
  dois sensores MPU-6050, um em cada meia asa, um sensor MPU-9250, no
  centro da aeronave, um GPS também no centro da aeronave, um botão
  para inicialização do sistema e um buzzer para sinalizar a condição
  do sistema.
  
  (i) O buzzer apitará um toque uma única vez para sinalizar que o sistema foi inicilizado com sucesso; 
  
  (ii) Apitará diversas vezes por alguns segundos afim de sinalizar que ocorreu algum erro no sistema que deve ser analisado no terminal; 
  
  (iii) Apitará uma única vez, por meio segundo, para sinalizar que o sistema foi desligado. Neste momento, os dados serão salvos antes do código sinalizar a terminação da presente sessão da raspberry; 
  
  (iv) ...   
  
  =======================================================================
  Para rápida referência, as structs estão enumeradas da seguinte forma:
 
  meiaAsaDireita = imu_struct[0]
  meiaAsaEsquerda = imu_struct[1]
  Aviao = imu_struct[2]
# ======================================

Por enquanto o código não é compilável.

Isto se deve à inicialização da biblioteca RTIMULib que necessita ser instalada e compilada por meio de um makefile.
Próximas iterações de projeto possuirão um makefile decente para execução de testes.
