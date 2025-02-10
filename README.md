# Tarefa 02/03 - Interfaces de Comunicação Serial

Este projeto tem como objetivo explorar a comunicação serial no microcontrolador RP2040 utilizando a placa de desenvolvimento BitDogLab. Serão utilizadas as interfaces UART e I2C para exibição de caracteres em um display SSD1306 e interação com LEDs e botões físicos.

## Componentes utilizados

Matriz 5x5 de LEDs WS2812 (GPIO 7)

LED RGB (GPIOs 11, 12 e 13)

Botão A (GPIO 5)

Botão B (GPIO 6)

Display SSD1306 via I2C (GPIOs 14 e 15)

## Funcionalidades implementadas

1. Modificação da Biblioteca font.h

Adicionadas letras minúsculas personalizadas para exibição no display SSD1306.

2. Entrada de Caracteres via Serial

O Serial Monitor do VS Code é utilizado para entrada de caracteres.

Cada caractere digitado é exibido no display SSD1306.

Se um número de 0 a 9 for digitado, um símbolo correspondente é mostrado na matriz WS2812.

3. Interação com o Botão A

Alterna o estado do LED RGB verde.

Exibe uma mensagem informativa no display SSD1306.

Envia um texto descritivo para o Serial Monitor.

4. Interação com o Botão B

Alterna o estado do LED RGB azul.

Exibe uma mensagem informativa no display SSD1306.

Envia um texto descritivo para o Serial Monitor.

## Como Executar o Projeto

Clone o repositório do projeto para o seu ambiente local e abri-lo no VS Code.

Antes de compilar o código, é necessário garantir que todas as extensões e ferramentas estão devidamente instaladas.

Instalar o Pico SDK e extensão Raspberry Pi Pico: Siga as instruções do Pico SDK para configurar o ambiente de desenvolvimento no VS Code e instale a extensão Raspberry Pi Pico.

Compilar e carregar o código na placa BitDogLab(Para ter melhores resultados, utilizar somente com a BitDogLab, ou utilizar os componentes nescessários conectados as GPIOs corretas indicadas em tópicos anteriores).

Abrir o Serial Monitor e interagir com o sistema digitando caracteres(BitDogLab tem que estar conectada ao USB).

Testar os botões para alternar o estado dos LEDs e visualizar as mensagens no display e no Serial Monitor.

## Link para video demonstração:

https://drive.google.com/drive/folders/1X52OR7T9Mqj292G0V7mNhHCejzXq6D06?usp=drive_link

# Projeto por : Alex Rodrigo Porto Barbosa
