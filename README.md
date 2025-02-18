# ADC

## Descrição

Este repositório contém os códigos correspondentes aos requisitos solicitados pela tarefa 1 do capítulo 8 da unidade 4 do programa de capacitação em sistemas embarcados (Embarcatech). A tarefa em questão busca incitar a utilização de Pulse Width Modulation (PWM) e de Analogic Digital Conversor (ADC) para controlar a intensidade de um led a partir do valor obtido pela leitura dos eixos X e Y de um joystick. Além disso, os mesmo valores de X e Y são utilizados para manipular um Display OLED. o programa faz uso de 2 botões, um que se encontra no joystick, responsável por alternar o estado de um LED, e o outro que se encontra na placa, responsável por habilitar / desabilitar o controle da intensidade dos LED's por meio do PWM usando o joystick.

## Requerimentos

O presente projeto deverá atender aos seguintes requerimentos:

* Uso de interrupções: Todas as funcionalidades relacionadas aos botões devem ser implementadas utilizando rotinas de interrupção (IRQ). 

* Debouncing: É obrigatório implementar o tratamento do bouncing dos botões via software.  

* Utilização do Display 128 x 64: A utilização de ferramentas gráficas demonstrará o entendimento do princípio de funcionamento do display, bem como, a utilização do protocolo I2C. 

* Organização do código: O código deve estar bem estruturado e comentado para facilitar o entendimento. 

## Demonstração

Segue o vídeo de demonstração do projeto:

https://github.com/user-attachments/assets/2e0b394d-a982-416e-a1c0-e43fe314623a

## Instruções

Para que o projeto funcione em sua máquina, é necessário ter a extensão Raspberry Pi Pico Project instalada no VSCode.

1. Realize o clone do repositório.
2. Importe o repositório como um projeto através da extensão Raspberry Pi Pico Project.
3. Compile o projeto.
4. Caso esteja usando a placa BitDogLab ou a Raspberry Pi Pico W, faça o upload do arquivo uf2, encontrado na pasta `build` após o processo de compilação, para a placa. Caso deseje simular, abra o arquivo `diagram.json` (É necessário ter a extensão do Wokwi instalada). Dentro do ambiente de simulação, os eixos do joystick estarão inverdidos, e o quadrado não estará centralizado, essas incronguências ocorrem devido às características de fabricação da placa BitDogLab, uma vez que o principal objetivo é que este projeto execute na mesma.
