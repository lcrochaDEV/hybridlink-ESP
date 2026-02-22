#ifndef SERVIDORWEB_H
#define SERVIDORWEB_H

#include <Arduino.h>
#include <ArduinoJson.h>
#if defined(ESP8266)
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


// Cria o objeto Servidor na porta 80 (porta HTTP padrão)
AsyncWebServer server(80);

// HTML armazenado na Flash
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Nexus Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { 
            
            font-family: sans-serif;            
            background: rgba(26, 13, 46, 0.95);
            color: #fff; 
        }

        .container_painel {
            display: flex;
            flex-direction: column; 
            align-items: flex-start;
            justify-content: flex-start;
            gap: 20px;  
            padding: 1.5em 2.5em; /* Aumentei um pouco o preenchimento lateral */
            position: relative;
            z-index: 0;
            border-radius: 10px;
            background: #11111113; 
            color: white;
            width: fit-content;
            margin: 5px;
        }

        /* Camada 1: O Gradiente Animado (Fica por baixo) */
        .container_painel:before {
            content: "";
            position: absolute;
            top: -2px;    /* Espessura da borda para fora */
            left: -2px;
            right: -2px;
            bottom: -2px;
            z-index: -2;
            background: linear-gradient(40deg, #ff0000, #ff7300, #fffb00, #48ff00, #00ffd5, #002bff, #7a00ff, #ff00c8, #ff0000);
            background-size: 400%;
            border-radius: 11px; /* Ligeiramente maior que o container */
            animation: glowing-scan 20s linear infinite;
            filter: blur(2px); /* Blur baixo para borda nítida, aumente para efeito neon */
        }

        /* Camada 2: O Fundo Interno (Corta o gradiente para parecer uma borda) */
        .container_painel:after {
            content: "";
            position: absolute;
            top: 1px;     /* Recuo para revelar a borda colorida */
            left: 1px;
            right: 1px;
            bottom: 1px;
            z-index: -1;
            background: #111111; /* Cor interna do painel */
            border-radius: 9px;
        }

        @keyframes glowing-scan {
            0% { background-position: 0 0; }
            50% { background-position: 400% 0; }
            100% { background-position: 0 0; }
        }

        /* Container e Rótulo */
        .container { 
            font-family: sans-serif; 
            margin-bottom: 20px; 
            display: flex; 
            align-items: center; 
            gap: 15px;
        }

        /* Estrutura do Switch */
        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 30px;
        }

        .switch input { 
            opacity: 0;
            width: 0;
            height: 0;
        }

        /* Cor quando está DESLIGADO (OFF) - Vermelho */
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0; left: 0; right: 0; bottom: 0;
            background-color: #f44336; /* Vermelho */
            transition: .4s;
            border-radius: 34px;
        }

        /* O círculo branco */
        .slider:before {
            position: absolute;
            content: "";
            height: 22px;
            width: 22px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }

        /* Cor quando está LIGADO (ON) - Verde */
        input:checked + .slider { 
            background-color: #4CAF50; /* Verde */
        }

        /* Movimento do círculo */
        input:checked + .slider:before { 
            transform: translateX(30px); 
        }
</style>
</head>
<body>
    <h1>Painel de Controle</h1>
    <div class="container_painel coluna">
        <h3>ESP-8266</h3>
        <form id="action0" method="GET">
        <div class="container">
            <label>GPIO 0</label>
            <label class="switch">
            <input type="checkbox" name="gpio0" value="ON" checked>
            <span class="slider"></span>
            </label>
        </div>
        </form>

        <!-- GPIO 2 -->
        <form id="action2" method="GET">
        <div class="container">
            <label>GPIO 2</label>
            <label class="switch">
            <input type="checkbox" name="gpio2" value="ON" checked>
            <span class="slider"></span>
            </label>
        </div>
        </form>
    </div>
    <script>
        let action0 = document.querySelector('#action0');
        let gpioZaro = document.querySelector('input[name=gpio0]');
        
        window.addEventListener('load', () => cmd(0,1));
        window.addEventListener('load', () => cmd(2,1));
        
        action0.addEventListener('change', (event) => {
            event.preventDefault();
            console.log(event.target.form.elements['gpio0'])
            let gpioZero = event.target.form.elements['gpio0'];
            if(gpioZaro.value === 'ON'){
                gpioZero.value = "OFF"
                 cmd(0,0)
            }else {
                gpioZero.value = "ON"
                 cmd(0,1)
            }
        })
        let action2 = document.querySelector('#action2');
        let gpioDois = document.querySelector('input[name=gpio2]');
        action2.addEventListener('change', (event) => {
            event.preventDefault();
            let gpioDois = event.target.form.elements['gpio2'];
            if(gpioDois.value === 'ON'){
                gpioDois.value = "OFF"
                cmd(2,0)
            }else {
                gpioDois.value = "ON"
                cmd(2,1);
            }
        })
        function cmd(p, e) {
            fetch('/controlar', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({pino: p, estado: e})
            });
        }
    </script>
</body>
</html>
)rawliteral";

void startServer() {
    // Configuração de Rotas (Endpoints)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    // Rota de Controle (JSON)
    server.on("/controlar", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, 
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        JsonDocument doc;
        deserializeJson(doc, data, len);
        int pino = doc["pino"];
        int estado = doc["estado"];
        
        if (pino == 0 || pino == 2) {
            digitalWrite(pino, estado);
            Serial.printf("Acionado o Pino: %d com estado: %d\n", pino, estado);
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(400, "application/json", "{\"status\":\"erro\"}");
        }
    });
  // Inicia o Servidor 
  server.begin();
  Serial.println("Servidor HTTP Async Iniciado!");
}

#endif