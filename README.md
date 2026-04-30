
# Nexus Framework — Sistema de Gestão IoT para ESP32 & ESP8266 (HybridLink-ESP)

Este projeto é um framework robusto para microcontroladores ESP, projetado para oferecer controle dinâmico de pinos GPIO, persistência de estados via sistema de arquivos (LittleFS) e uma interface web assíncrona moderna.

## 🚀 Funcionalidades principais

- **Persistência de Dados**: Salva estados de pinos, modos e vínculos em arquivos JSON no LittleFS.
- **Interface Web Moderna**: Painel de controle responsivo (HTML5/JS) para gerir hardware em tempo real sem recarregar a página.
- **Vínculos Lógicos (Pin Mirroring)**: Permite que um pino (ex: entrada de botão) espelhe seu estado para outro pino (ex: saída de relé) via software.
- **Gestão de Botões Profissional**: Classe `FootSwetch` com suporte a debounce, clique curto, médio e longo.
- **Conectividade Inteligente**: Utiliza `WiFiManager` para configuração dinâmica de Wi-Fi e `ESPAsyncWebServer` para alta performance.
- **Arquitetura Modular**: Separação clara entre camadas de Hardware (Physical), Persistência (FileSystem) e Comunicação (Wireless).

## 📂 Estrutura do Projeto

| Classe / Módulo | Descrição |
| :--- | :--- |
| `AccessControl` | Orquestrador principal que une a lógica de hardware à persistência. |
| `FileSystemControl` | Gere a leitura/escrita do `config.json` e sincroniza o hardware no boot. |
| `PhysicalAccessControl` | Camada de abstração de hardware (HAL) para métricas de RAM e GPIOs. |
| `FootSwetch` | Lógica de detecção de tempo e ações para interruptores físicos. |
| `WirelessConnection` | Gere a conexão Wi-Fi e modos de Access Point. |
| `Console` | Interface de linha de comando (CLI) via Serial para depuração. |

## 🛠️ Tecnologias Utilizadas

- **Framework**: Arduino Core (C++)
- **JSON**: [ArduinoJson](https://arduinojson.org/)
- **Servidor Web**: [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- **Sistema de Arquivos**: LittleFS
- **Front-end**: JavaScript Puro (Vanilla JS) com Fetch API

## 📋 Como Utilizar

1. **Configuração da IDE**:
   - Certifique-se de selecionar a partição correta para LittleFS (pelo menos 1MB recomendado).
   - Instale as bibliotecas `ArduinoJson` e `ESPAsyncWebServer`.

2. **Endpoints da API**:
   - `GET /status`: Retorna o JSON atual com os estados reais de todos os pinos.
   - `POST /controlar`: Altera o estado (HIGH/LOW) de um pino.
   - `POST /config_modo`: Altera o modo do pino (INPUT, OUTPUT, PULLUP).
   - `POST /config_link`: Cria um vínculo lógico entre dois pinos.

3. **Comandos de Console (Serial)**:
   - `SHOWDATA`: Exibe a configuração atual no terminal.
   - `ERASEDATA`: Realiza o reset de fábrica (apaga o JSON e reinicia).
   - `HELP`: Lista comandos disponíveis.

---

##  🔧 Estrutura de Classe

Este documento fornece uma visão geral técnica das classes, métodos e funções do projeto.

## 1. Classe `FileSystemControl`
Responsável pelo gerenciamento do sistema de arquivos (LittleFS) e persistência de dados JSON.

* ### 1. Inicialização e Sincronização

   - **begin()**: Monta o sistema de arquivos LittleFS. Se a montagem falhar, ele verifica se o tamanho da memória Flash está correto e tenta formatar a partição automaticamente. Ao final, chama a sincronização de hardware.  
   - **syncHardwareWithDisk()**: Método vital executado no boot. Ele lê o arquivo config.json e configura fisicamente cada pino registrado (definindo pinMode e digitalWrite) para que o hardware retorne exatamente ao estado em que estava antes de ser desligado. 

* ### 2. Manipulação de Arquivos (JSON)

   - **loadConfig(JsonDocument& doc)**: Abre o arquivo de configuração para leitura e converte o conteúdo de texto para um objeto JSON manipulável na memória RAM.  
   - **saveConfig(JsonDocument& doc)**: Realiza o processo inverso: converte o objeto JSON da RAM para texto e o grava fisicamente no LittleFS. Inclui um pequeno delay(50) para garantir que a escrita na memória Flash seja concluída com segurança.  
   - **fillJson(...)**: Uma função auxiliar simples para preencher os campos de SSID e Senha do Wi-Fi dentro de um documento JSON

* ### 3. Configuração de Pinos (Interface de Usuário)

   Estes métodos utilizam a técnica de "Patch", alterando apenas uma característica específica de cada vez através da função interna _addPinConfig:

   - **setPinMode(pin, mode)**: Define se o pino é entrada, saída ou pull-up, preservando o estado e o nível atual.  
   - **setPinLevel(pin, level)**: Define o nível lógico (HIGH/LOW) para pinos de saída.  
   - **setPinState(pin, state)**: Atualiza o estado lógico/físico do pino no registro.  
   - **setPinLink(pin, vincularpin)**: Estabelece o vínculo lógico entre dois pinos para espelhamento.

* ### 4. Lógica Interna e Gestão

   - **_addPinConfig(...)**: O motor de processamento da classe. Ele busca um pino existente no array JSON ou cria um novo se não encontrar. Graças ao uso dos sentinelas MODE_KEEP, PIN_KEEP e LINK_KEEP, ele consegue atualizar apenas o campo solicitado (como o state) sem apagar ou resetar os outros campos (como o vincularpin).  
   - **getLinkedPin(originPin)**: Varre o arquivo de configuração em busca de qualquer pino que esteja configurado para "seguir" o pino de origem informado. É essencial para a automação interna do espelhamento.  
   - **credentials()**: Verifica se as novas credenciais de Wi-Fi são diferentes das já salvas antes de realizar uma nova escrita, o que ajuda a poupar a vida útil da memória Flash (que possui ciclos limitados de gravação).  
   - **returnObjectData()**: Um método de diagnóstico que imprime o conteúdo completo do arquivo de configuração no Monitor Serial de forma formatada ("Pretty Print").  
   - **factoryReset()**: Método de "limpeza total". Deleta o arquivo de configuração e reinicia o chip (ESP.restart()), retornando o dispositivo ao estado de fábrica. 

* ## 5. Espelhamento de Pinos.

   - **runMirroring**: é o "coração" da sincronização de hardware no seu sistema. Ele permite que um pino (o Escravo) siga automaticamente o estado de outro pino (o Mestre), garantindo que ambos funcionem em uníssono, método opera seguindo uma lógica de "vigilância" constante:
   
   - **Fonte da Verdade**: Ele utiliza o arquivo JSON (armazenado no LittleFS) como a referência principal para saber quem manda em quem.  
   - **Identificação de Vínculos**: O código percorre a lista de pinos e procura pela chave vincularpin. Se um pino tem um valor de vínculo diferente de -1, ele é identificado como um escravo.  
   - **Sincronização de Estado**: Ao encontrar um escravo, o método localiza o pino mestre correspondente, lê o seu estado atual no objeto JSON e aplica esse mesmo estado ao pino físico do escravo usando digitalWrite.  
   - **Atualização do Objeto**: Para que o seu painel web (front-end) mostre a mudança, o método também atualiza o campo state do escravo no próprio objeto JSON em memória.  

   ### Por que ele é importante?

   Sem esse método, o espelhamento seria apenas estático. O runMirroring transforma as configurações salvas em ações de hardware em tempo real, permitindo que um clique em um único botão no seu site acione múltiplos componentes físicos de forma coordenada.  

   


## 2. Classe `PhysicalAccessControl` 

A classe `PhysicalAccessControl` fornece uma camada de abstração para identificar as capacidades físicas e o estado do hardware do microcontrolador, sendo compatível tanto com **ESP8266** quanto com **ESP32**.

Abaixo, detalho seus métodos e funções:

* ### 1. Identificação e Diagnóstico do Hardware

   *   **modelBoardESP()**: Identifica o modelo da placa. No ESP8266, retorna uma string fixa "ESP8266 ESP-01", enquanto no ESP32 utiliza a função nativa `ESP.getChipModel()` para uma identificação precisa.
   *   **total_ran()**: Estima ou lê a capacidade total de memória RAM (Heap). Para o ESP8266, realiza um cálculo aproximado somando o heap livre a uma constante, enquanto no ESP32 utiliza o tamanho real do heap via `ESP.getHeapSize()`.
   *   **flash_size()**: Retorna a capacidade da memória Flash em megabytes (MB). O método ajusta a chamada de sistema (`getFlashChipRealSize` vs `getFlashChipSize`) dependendo da arquitetura detectada.
   *   **menor_ran_size()**: Informa a quantidade de memória RAM atualmente livre em kilobytes (KB).
   *   **sketch_Size()**: Retorna o tamanho do binário (sketch) atual carregado no chip em KB.

* ### 2. Mapeamento de Pinos (GPIO)

   **`pinGPIO()`**: Este é um dos métodos mais importantes para a integração com a interface Web. Ele gera uma string em formato JSON contendo o mapeamento dos pinos disponíveis para controle:
   *   **ESP8266**: Mapeia os pinos básicos GPIO0 e GPIO2.
   *   **ESP32**: Mapeia uma lista mais ampla de pinos (0, 2, 4, 5, 18, 19, 21 e 22), aproveitando a maior densidade de IO do chip.

* ### 3. Utilidade no Sistema

   Esta classe fornece a base para que os endpoints de controle (como o `/controlar` que discutimos anteriormente) validem se um pino enviado pelo usuário realmente existe no hardware. Ela garante a **portabilidade** do seu código, permitindo que o mesmo software rode em diferentes gerações de chips da Espressif com ajustes automáticos de configuração.

## 3. Classe `Console`

   A classe `Console` estende a funcionalidade do seu sistema ao herdar de `AccessControl`, adicionando uma interface de linha de comando (CLI) via Monitor Serial para interação direta com o hardware e o sistema de arquivos.

   Aqui está a análise detalhada dos seus métodos e funções:

   * ### 1. Estrutura e Construtor
      -  **Herança**: A classe herda de `AccessControl`, o que significa que o objeto `Console` possui todas as capacidades de controle físico, gestão de LittleFS e lógica de espelhamento interno.
      -  **Console(const char\* consoleText)**: O construtor inicializa a classe base `AccessControl` e armazena uma mensagem de texto padrão para ser usada em saídas de console.

  *  ### 2. Métodos de Mensagem e Inicialização
      - **helloWord(const char\* consoleText)**: Exibe uma mensagem de boas-vindas no terminal. Ele prioriza o texto passado como argumento; se for nulo, utiliza a mensagem padrão definida no construtor.
      - **menssageViewMsg(const char\* consoleText)**: Uma função auxiliar simples que encapsula o `Serial.println`, padronizando como as mensagens são enviadas para o Monitor Serial.

   * ### 3. Sistema de Terminal Interativo
   
   * **`consoleView()`**: Este é o motor de escuta do terminal que deve ser chamado no `loop()` principal. Ele verifica se há dados no buffer serial, lê a entrada até a quebra de linha (`\n`) para evitar atrasos de *timeout*, remove espaços em branco e converte o comando para letras maiúsculas antes de processá-lo.

   * **`commands_envio(const String& command)`**: Atua como o processador de comandos (parser). Ele imprime o comando digitado com um prefixo personalizado ("Mochi> ") e executa ações baseadas na entrada do usuário:

      - **"HELP"**: Exibe a lista de comandos disponíveis.
      - **"ERASEDATA"**: Invoca o método `factoryReset()` (herdado de `FileSystemControl`), que deleta o arquivo de configuração e reinicia o ESP.
      - **"SHOWDATA"**: Invoca o método `returnObjectData()` (também herdado), imprimindo o conteúdo do arquivo JSON formatado no serial.
      - **Comando Inexistente**: Informa ao usuário que o comando não foi reconhecido.

   ### Resumo da Integração
   Com esta classe, o seu projeto ganha uma ferramenta de diagnóstico poderosa. Enquanto o `runMirroring` processa o espelhamento de pinos em segundo plano e o Servidor Web atende requisições externas, o `Console` permite que você monitore o estado interno do JSON (`SHOWDATA`) ou limpe as configurações do sistema (`ERASEDATA`) manualmente através do cabo USB.

## 4. Funções Globais (Main)
- **`setupFS()`**: Prepara o sistema de arquivos no início do programa.

## 5. AccessControl

A classe AccessControl atua como a camada de integração principal do seu framework, utilizando o conceito de herança múltipla para consolidar as funcionalidades de gerenciamento de hardware e persistência de dados.  

Abaixo, comento a estrutura e o papel desta classe:

* ### 1. Estrutura e Herança

   - **Herança Múltipla**: A classe herda de PhysicalAccessControl e FileSystemControl. Isso significa que um objeto do tipo AccessControl possui, simultaneamente, todos os métodos de manipulação física de pinos e todos os métodos de gerenciamento de arquivos JSON e LittleFS.  
   - **Papel de Orquestradora**: Ela serve como o ponto de acesso único para o seu programa principal. Em vez de instanciar várias classes, você utiliza apenas a AccessControl para configurar o Wi-Fi, salvar pinos no disco e executar o espelhamento.  

* ### 2. Construtor
   - **AccessControl(...)**: O construtor recebe as credenciais de rede (ssid e password) e as repassa diretamente para o construtor da classe base FileSystemControl através da lista de inicialização. Isso garante que, ao criar o objeto, as configurações de rede já estejam disponíveis para os métodos que gerenciam o arquivo de configuração.  

* ### 3. Métodos Herdados (Principais Funcionalidades)

   Como a classe é uma extensão das anteriores, ela disponibiliza as funções que discutimos:

   - **Gestão de Espelhamento**: Através da herança, ela expõe o runMirroring() para o loop() e o updateMirrorMap() para sincronização de memória.  
   - **Persistência de Estados**: Permite o uso de setPinState(), setPinLink() e setPinMode(), que garantem que as alterações feitas via interface Web (como no seu endpoint /controlar) sejam gravadas no disco e aplicadas ao hardware imediatamente.  
   - **Sincronização de Hardware**: Utiliza o syncHardwareWithDisk() para garantir que, após uma queda de energia, o ESP32 retome exatamente o estado físico e os vínculos salvos no arquivo JSON.  
   
### Resumo da Organização
   A classe está organizada de forma modular:

   - **PhysicalAccessControl**: Provavelmente lida com a abstração dos pinos GPIO e leitura/escrita física.  
   - **FileSystemControl**: Lida com a inteligência do JSON, LittleFS e a lógica de espelhamento em RAM.  
   - **AccessControl**: Une essas duas forças em uma única interface simplificada para o desenvolvedor.  

## 6. FootSwetch

   A classe `FootSwetch` foi projetada para gerenciar botões (pedais) de forma inteligente e não-bloqueante, permitindo que um único pino físico execute até três ações diferentes baseadas no tempo de pressão.

   Abaixo estão os detalhes da classe, seus métodos e funcionamento:

* ### 1. Estrutura e Configuração
   *  **SwetchActions (Struct)**: Define três tipos de retornos de chamada (callbacks) que podem ser vinculados ao botão: curto (atbaixo de 500ms), médio (entre 500ms e 5000ms) e longo (acima de 5000ms).  
   * **Configuração de Hardware: O pino do botão é configurado automaticamente como INPUT_PULLUP, eliminando a necessidade de resistores externos (o botão deve conectar o pino ao GND ao ser pressionado).

* ### 2. Métodos Principais
   *   **Construtor `FootSwetch(pin, actions)`**: Inicializa o pino do botão, as ações desejadas e reseta as variáveis de estado interno para uma inicialização segura.
   *   **`setFeedbackPin(targetPin)`**: Define um pino de saída opcional que serve como indicador visual ou de "identificação". Este pino "segue" o estado físico do botão em tempo real: se o botão for pressionado, o pino de feedback liga; se solto, desliga.
   *   **pinAction()**: É o coração da classe e deve ser chamado continuamente dentro do `loop()`. Ele monitora o estado do pino e gerencia o tempo sem usar `delay()`, garantindo que o programa não trave.
   *   **setActions(newActions)**: Permite alterar dinamicamente as funções vinculadas ao botão durante a execução do programa.

* ### 3. Lógica de Detecção de Pressionamento
   A função `pinAction()` divide o comportamento do botão em três momentos distintos:
   1.  **Momento do Aperto**: Registra o tempo de início (`millis()`) e reseta os sinalizadores de execução.
   2.  **Durante a Pressão**: Monitora se o tempo ultrapassou o limite de "Longo" (5 segundos). Se ultrapassar, a ação `onLong` é executada imediatamente, mesmo antes de o usuário soltar o botão.
   3.  **Momento de Soltar**: Se o botão for solto antes dos 5 segundos, a classe calcula a duração total e decide se dispara a ação `onShort` ou `onMedium`, respeitando um tempo de *debounce* de 50ms para evitar falsos acionamentos.

* ### 4. Diferenciais Técnicos
   *   **Não-Bloqueante**: Diferente de implementações simples, esta classe permite que outras partes do código (como o seu Servidor Web e o `runMirroring`) continuem rodando enquanto você segura o pedal.
   *   **Ações Customizáveis**: Cada ação recebe o número do pino como argumento (`int pin`), permitindo que a mesma função de *callback* identifique qual pedal a disparou.
   *   **Feedback Visual Integrado**: A capacidade de controlar um pino de feedback diretamente na lógica de detecção facilita a criação de interfaces físicas intuitivas.

## 7. WifiConnect

A classe `WifiConnect` é responsável por gerenciar a conectividade de rede do dispositivo, oferecendo suporte tanto para conexão manual via credenciais quanto para configuração automática através de um Portal de Captiveiro.

Abaixo estão os detalhes de seus métodos e funcionalidades:

 * ### 1. Estrutura e Configuração de Portabilidade
   *   **Compatibilidade Multiplataforma**: A classe utiliza diretivas de compilação condicional (`#if defined`) para incluir automaticamente as bibliotecas corretas (`ESP8266WiFi.h` ou `WiFi.h`) dependendo da placa utilizada (ESP8266 ou ESP32).
   *   **Construtor `WifiConnect`**: Inicializa as variáveis protegidas `ssid` e `password`. Se nenhum valor for passado, elas são definidas como nulas, permitindo flexibilidade entre conexão manual ou via Access Point.

* ### 2. Métodos de Conexão
   **`connectionsMethod()`**: Realiza a tentativa de conexão manual à rede Wi-Fi.
   *   **Limpeza de Rádio**: Força a desconexão total antes de iniciar para garantir um estado limpo.
   *   **Resiliência**: Configura a reconexão automática e define que as configurações não devem ser persistidas na memória flash (`persistent(false)`) para evitar desgaste do hardware.
   *   **Controle de Tentativas**: Possui um loop que aguarda a conexão por até 20 tentativas (definidas por `maxTentativas`), evitando que o código fique travado infinitamente caso a rede esteja indisponível.
   
   **`accesspoint()`**: Utiliza a biblioteca `WiFiManager` para criar um ponto de acesso de configuração.
   *   **Identificação Única**: Gera um SSID automático baseado no endereço MAC do dispositivo (ex: `ESP:AABBCCDDEEFF`), facilitando a identificação de múltiplos módulos.
   *   **Portal de Configuração**: Abre um portal onde o usuário pode conectar seu celular e configurar o Wi-Fi sem precisar reprogramar o código.

* ### 3. Monitoramento e Status
   **`connect_status()`**: Valida se a conexão está ativa e fornece diagnósticos via Monitor Serial.   
   *   **Informações Detalhadas**: Se conectado, imprime o nome da rede (SSID), endereço IP local, endereço MAC e o canal Wi-Fi atual.
   *   **Retorno Booleano**: Retorna `true` para sucesso ou `false` para desconexão, permitindo que outras partes do sistema (como o Servidor Web) saibam se podem iniciar suas operações.

* ### Variáveis Privadas e Protegidas
   *   **`maxTentativas` (20)**: Define o limite de paciência do sistema ao tentar conectar manualmente.
   *   **`ssid` e `password`**: Armazenam as credenciais de forma protegida para serem usadas pelos métodos de conexão da classe.

## 8. WirelessConnection

A classe `WirelessConnection` funciona como uma camada de especialização ou um "alias" para a gestão de rede, herdando diretamente todas as capacidades da classe `WifiConnect`.

Aqui estão os comentários sobre sua estrutura e finalidade:

* ### 1. Herança e Estrutura
   *   **Herança Simples**: A classe herda publicamente de `WifiConnect`. Isso significa que qualquer objeto instanciado como `WirelessConnection` tem acesso total aos métodos `connectionsMethod()`, `accesspoint()` e `connect_status()` definidos na classe base[cite: 11, 14].
   *   **Objetivo de Design**: Frequentemente, esse tipo de classe é utilizado para organizar o código sob uma nomenclatura mais semântica ou para preparar o terreno para futuras funcionalidades específicas de redes sem fio que não pertençam à lógica básica do Wi-Fi.

* ### 2. Construtor
   *   **`WirelessConnection(const char* ssid, const char* password)`**: O construtor recebe as credenciais de rede (opcionais, com padrão `nullptr`).
   *   **Repasse de Parâmetros**: Ele utiliza uma lista de inicialização para repassar o `ssid` e o `password` diretamente para o construtor da classe pai `WifiConnect`, garantindo que as credenciais sejam armazenadas corretamente desde o momento da criação do objeto.

* ### 3. Funcionalidades Disponíveis (via Herança)

   Como a classe não define novos métodos nos arquivos fornecidos, sua funcionalidade é idêntica à de `WifiConnect`:

   *   **Conexão Manual**: Capacidade de conectar a um roteador específico usando `connectionsMethod()`.
   *   **Portal de Configuração**: Capacidade de iniciar um Access Point para configuração via navegador usando `accesspoint()`.
   *   **Diagnóstico**: Verificação de IP, MAC e força do sinal através do método `connect_status()`.

#  🌐 servidorweb (Sevidor Web) 

os endpoints foram construídos para serem o cérebro da comunicação entre a sua interface Web e o hardware. Eles utilizam a biblioteca ESPAsyncWebServer, o que é excelente para não bloquear o loop principal do ESP.

Aqui estão os comentários sobre os principais endpoints identificados:

### 1. GET /

* Finalidade: Entrega a interface visual (Frontend).
* Destaque: Utiliza a função processor para injetar dados dinâmicos como o SSID, IP e MAC diretamente no HTML antes de enviá-lo ao navegador.

### 2. POST /config_pinos

* Finalidade: Cadastro e atualização de periféricos.
* Ação: Recebe o JSON com os detalhes do pino (número, modo, nível inicial) e utiliza o accessSys.addPinConfig() para salvar essas definições no config.json. É aqui que o sistema "toma conhecimento" de que um novo pino será usado.

### 3. GET /controlar

* Finalidade: Acionamento manual via Web.
* Ação: Altera o estado (state) de um pino de saída. Se você clicar em um botão na interface para "Ligar", este endpoint é chamado, executa o digitalWrite e atualiza o estado no arquivo de configuração para que o ESP lembre dele após um reboot.

### 4. GET /config_modo

* Finalidade: Alteração dinâmica de comportamento (INPUT, OUTPUT, PULLUP).
* Ação: Reconfigura o pinMode do hardware em tempo real e salva a nova preferência no disco.

### 5. GET /set_level

* Finalidade: Define a lógica de atuação (Normalmente Aberto / Normalmente Fechado).
* Ação: Inverte o level (0 ou 1) que o sistema considera como "Ativo", permitindo que o mesmo código funcione para relés que ativam em nível baixo (LOW) ou alto (HIGH).

### 6. GET /status (A "Fonte da Verdade")

* Finalidade: Sincronização entre Hardware e Interface.
* Ação: Este é o endpoint mais crítico para o monitoramento. Ele lê o arquivo config.json, mas antes de enviar para a Web, ele executa um digitalRead físico em cada pino para garantir que o que você vê na tela reflete o estado elétrico real do componente.

### 7. POST /config_link (O Vínculo Inteligente)

* Finalidade: Estabelecer a ponte lógica entre dois pinos.
* Ação: Recebe o pino de origem (target) e o pino alvo (pin). Salva essa relação no campo vincularpin do JSON através do método accessSys.setPinLink.

## Sobre a sua dúvida da inicialização:

Como esses endpoints apenas salvam ou alteram os dados no config.json, o C++ só passará a agir conforme o novo "vínculo" em duas situações:

1. No Reboot: Quando o setupFS() rodar, ele lerá o novo JSON.
2. Na Atualização a Quente: Se você adicionar no POST /config_link uma chamada para o método setFeedbackPin() do seu objeto FootSwetch.

**Resumo**: O arquivo servidorweb.h atua como uma ponte. Ele não executa a lógica de controle diretamente no loop, mas prepara os dados para que as classes AccessControl e FootSwetch saibam o que fazer. Se o vínculo for configurado via /config_link, o FileSystemControl garante que essa informação não seja perdida se a energia cair.


*Desenvolvido por lcrochaDEV — Focado em performance e estabilidade para sistemas embarcados.*