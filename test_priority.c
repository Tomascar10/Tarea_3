#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int i;

  for (i = 0; i < 20; i++) {
    int pid = fork();
    if (pid == 0) { // Proceso hijo
      printf("Ejecutando proceso %d\n", i);
      sleep(100);   // Pausa para simular la ejecución del proceso
      exit(0);
    }
  }

  // Esperar a que todos los hijos terminen
  for (i = 0; i < 20; i++) {
    wait(0);
  }

  exit(0);
}
