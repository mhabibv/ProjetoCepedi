## Projeto de Inclusão para Deficientes Visuais: Semáforo Sonoro

### Escopo do Projeto

Este projeto visa proporcionar uma solução inclusiva para deficientes visuais, permitindo que eles possam identificar o estado atual de um semáforo (vermelho, amarelo ou verde) por meio de um sistema de som. A principal função do sistema é fornecer uma indicação sonora correspondente ao estado atual do semáforo quando um botão é pressionado, possibilitando que o usuário saiba se o semáforo está vermelho, amarelo ou verde.

O sistema é composto por uma **Raspberry Pi Pico**, um **botão de entrada** para interação, um **buzzer** para gerar os sons correspondentes e LEDs para indicar visualmente o estado do semáforo. O semáforo alterna automaticamente entre as cores a cada 3 segundos, e o botão permite ao usuário obter a informação sonora a qualquer momento.

---

### Componentes de Hardware Utilizados

- **Raspberry Pi Pico**  
  - Microcontrolador que gerencia todo o sistema, realizando as leituras dos botões, controlando os LEDs e gerando os sinais PWM para o buzzer.

- **Botão (Pino GPIO 6)**  
  - Utilizado para interagir com o sistema. Quando pressionado, aciona a emissão do som correspondente ao estado atual do semáforo.

- **LEDs (Pinos GPIO 11, 12, 13)**  
  - LEDs para representar as cores do semáforo (vermelho, amarelo e verde).
    - **LED 13:** Representa o semáforo vermelho.
    - **LED 12:** Representa o semáforo amarelo.
    - **LED 11:** Representa o semáforo verde.

- **Buzzer (Pino GPIO 21)**  
  - Dispositivo de saída sonora utilizado para emitir os sinais audíveis correspondentes aos estados do semáforo.
    - **1kHz** para o semáforo vermelho.
    - **1.5kHz** para o semáforo amarelo.
    - **2kHz** para o semáforo verde.

---

### Software

O software foi desenvolvido para controlar a alternância do semáforo entre as cores (vermelho, amarelo e verde) e gerar a indicação sonora correspondente quando o botão for pressionado.

#### Funções e Procedimentos

- **`configurar_leds()`**  
  - Configura os LEDs do semáforo como saídas e os inicializa como apagados.

- **`configurar_buzzer()`**  
  - Configura o pino do buzzer para gerar sinais PWM, permitindo a variação da frequência do som.

- **`emitir_som(frequencia, duracao_ms)`**  
  - Emite um som no buzzer com a frequência e duração especificadas.

- **`emitir_som_correspondente()`**  
  - Emite o som correspondente ao estado atual do semáforo:
    - **Vermelho:** Som contínuo de 1kHz.
    - **Amarelo:** 3 bipes curtos de 1.5kHz.
    - **Verde:** 5 bipes rápidos de 2kHz.

- **`alterar_sinal(struct repeating_timer *t)`**  
  - Altera o estado do semáforo a cada 3 segundos, acendendo o LED correspondente à cor do semáforo atual e apagando os outros LEDs.

- **`main()`**  
  - Inicializa todos os componentes do sistema (LEDs, buzzer e botão). No loop principal, monitora o botão para emitir o som correspondente quando pressionado.

---


### Procedimento de Funcionamento

1. O sistema alterna automaticamente entre os estados do semáforo (**vermelho, amarelo, verde**) a cada 3 segundos.
2. Quando o usuário pressiona o botão, o sistema emite o som correspondente ao estado atual do semáforo.
3. O som varia conforme a cor do semáforo:
    - **Vermelho:** Som contínuo de 1kHz.
    - **Amarelo:** 3 bipes curtos de 1.5kHz.
    - **Verde:** 5 bipes rápidos de 2kHz.

---

### Considerações Finais

Este sistema oferece uma alternativa acessível para deficientes visuais, permitindo que eles obtenham informações sobre o estado do semáforo através de sons, promovendo mais segurança e inclusão.

---
## Link demonstração

https://youtu.be/XC6w5YpyXZM?feature=shared
