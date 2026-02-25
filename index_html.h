#ifndef INDEX_HTML_H
#define INDEX_HTML_H

// HTML armazenado na Flash
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Painel Nexus</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        /* Ajuste para tablets e celulares */
        @media (max-width: 768px) {
            header {
                padding: 10px;
            }

            .module {
                flex-direction: column; /* Empilha a imagem e os dados */
                align-items: center;
                text-align: center;
            }

            .module img {
                height: 80px; /* Reduz o logo em telas menores */
                margin-bottom: 15px;
                margin-right: 0;
            }

            .module-itens {
                padding-right: 0;
                margin-bottom: 10px;
                width: 100%%;
            }

            .container_painel {
                width: 90%%; /* Ocupa quase a tela toda lateralmente */
                max-width: none;
                margin: 10px auto; /* Centraliza o painel */
                padding: 1.5em 1em;
            }

            h1 { font-size: 22px; text-align: center; }
            h3 { text-align: center; width: 100%%; }
        }

        /* Ajuste fino para celulares muito pequenos (iPhone SE, etc) */
        @media (max-width: 380px) {
            .container {
                gap: 5px;
            }
            
            .module-itens p {
                font-size: 12px; /* Diminui a fonte para não quebrar linha */
            }

            .switch {
                transform: scale(0.9); /* Reduz levemente o switch se o espaço for crítico */
            }
        }
        body { 
            margin: 0;
            padding: 0;
            font-family: sans-serif;            
            background: rgba(26, 13, 46, 0.95);
            color: #fff; 
        }
        header { background-color: #3d216b; }
        .module {
            display: flex;
            justify-content: flex-start;
            padding: 5px;
            font-size: 14px;
        }
        .module img {
            height: 130px;
            margin-right: 10px;
        }
        .module-itens { padding-right: 40px; }        

        .container_painel {
            display: flex;
            flex-direction: column; 
            gap: 10px;  
            position: relative;
            z-index: 0;
            border-radius: 10px;
            background: #11111113; 
            color: white;
            width: fit-content;
            margin: 10px;
            
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
            background-size: 400%%;
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
            z-index: -2;
            background: #111111; /* Cor interna do painel */
            border-radius: 9px;
        }

        @keyframes glowing-scan {
            0%% { background-position: 0 0; }
            50%% { background-position: 400%% 0; }
            100%% { background-position: 0 0; }
        }

        .container_painel h1 {
            background-color: rgba(62, 29, 100, 0.301);
            margin: 0;
            border-top-left-radius: 11px; 
            border-top-right-radius: 11px; 
            padding: 5px;   
            position: relative;
            z-index: 0;         
        }
        /* Container e Rótulo */
        .container, h3 { 
            display: flex;
            justify-content: space-between; 
            align-items: center; 
            padding: 0.1em .5em; /* Aumentei um pouco o preenchimento lateral */
        }
        .border {
            width: 100%%;
            border-bottom: #3a3b3a 1px solid;
            position: relative;
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
            background-image: url('https://cdn.iconscout.com/icon/free/png-256/free-espressif-logo-icon-svg-download-png-2285012.png?f=webp');
            background-size: cover; /* Ajusta a imagem ao tamanho da bolinha */ 
            transition: .4s;
            border-radius: 50%%;
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
    <header>
        <div class="module">
            <div class="module-img">
                <img src="https://cdn.iconscout.com/icon/free/png-256/free-espressif-logo-icon-svg-download-png-2285012.png?f=webp" />
            </div>
            <div class="module-itens"> 
                <p><span>SSID: </span>%SSID_VALUE%</p>
                <p><span>IP Address: </span>%IP_VALUE%</p>
                <p><span>MAC Address: </span>%MAC_VALUE%</p>
            </div>
            <div class="module-itens">
                <p><span>Total RAM Free: </span>%TOTAL_RAN_VALUE%</p>
                <p><span>Flash Size: </span>%FLASH_SIZE_VALUE%</p>
                <p><span>Menor RAM Free Heap Register: </span>%MENOR_RAN_SIZE_VALUE%</p>
                <p><span>Sketch Size: </span>%SKETCH_SIZE_VALUE%</p>
            </div>
        </div>
    </header>
    <div class="container_painel coluna">
        <h1>Painel Nexus</h1>
        <h3>ESP-8266</h3>
        <form id="action0" method="GET">
            <div class="container">
                <label>GPIO 00</label>
                <label class="switch">
                <input type="checkbox" name="gpio0" value="ON" checked>
                <span class="slider"></span>
                </label>
            </div>
        </form>
        <span class="border"></span>
        <!-- GPIO 2 -->
        <form id="action2" method="GET">
            <div class="container">
                <label>GPIO 02</label>
                <label class="switch">
                    <input type="checkbox" name="gpio2" value="ON" checked>
                    <span class="slider"></span>
                </label>
            </div>
        </form>
    <span class="border"></span>
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

#endif