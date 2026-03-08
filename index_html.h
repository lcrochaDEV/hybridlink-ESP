#ifndef INDEX_HTML_H
#define INDEX_HTML_H

// HTML armazenado na Flash
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Painel Nexus</title>
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
            padding-top: 4px;
            padding-left: 4px;
        }
        .module-itens { 
            padding-right: 40px;
            border-right: 1px solid #0303036e;
        }        
        .module-itens p {
            margin-left: 10px;
        }
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
            padding: .1em .5em; /* Aumentei um pouco o preenchimento lateral */
        }
        border {
            display: block;
            width: auto;
            border-bottom: #3a3b3a 1px solid;
            position: relative;
            margin: 8px 5px;
        }

        /* Estrutura do Switch */
        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 30px;
            margin-left: 20px;
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
        .container-radio {
            display: flex;
            align-items: center;
            cursor: pointer;
        }
        /* TABELA*/
        .gpio-table {
            width: 100%%;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            border-radius: 8px;
        }
        .gpio-table th {
            padding: 5px;
            text-align: center;
            background-color: rgba(62, 29, 100, 0.301);
        }
        .gpio-table td {
            padding: 10px;
            text-align: center;
        }
        .container-radio input {
            margin-right: 5px;
            margin-bottom: 3px;
            width: 18px;
            height: 18px;
            accent-color: #2196F3; /* Cor do seletor */
        }

    </style>
</head>
<body>
<body>
    <header>
        <div class="module">
            <div class="module-img">
                <img src="https://cdn.iconscout.com/icon/free/png-256/free-espressif-logo-icon-svg-download-png-2285012.png?f=webp" />
            </div>
            <div class="module-itens"> 
                <p><span>Modulo: </span>%MODULE_VALUE%</p>
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
        <h3>%MODULE_VALUE%</h3>
        <div class="conteiner-form">

        </div>
    </div>
    <script>

        let html_form = (list) => {
            return Object.keys(list).map(nome => {
                let pino = list[nome];
                return (`
                <form id="form_pin_${pino}" class="form-gpio">
                    <table class="gpio-table">
                        <thead>
                            <tr>
                                <th>State</th>
                                <th>Mode</th>
                                <th>Level</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td>
                                    <div class="container">
                                        <label>${nome}</label>
                                        <label class="switch">
                                            <input type="checkbox" name="gpio_${pino}" data-gpio="${pino}" class="switch-input" />
                                            <span class="slider"></span>
                                        </label>
                                    </div>
                                </td>
                                <td>
                                    <label class="container-radio">
                                        <input type="radio" name="mode_${pino}" value="OUTPUT" data-gpio="${pino}" class="mode-input" checked />
                                        <span class="label-text">OUTPUT</span>
                                    </label>
                              
                                    <label class="container-radio">
                                        <input type="radio" name="mode_${pino}" value="INPUT" data-gpio="${pino}" class="mode-input" />
                                        <span class="label-text">INPUT</span>
                                    </label>
                          
                                    <label class="container-radio">
                                        <input type="radio" name="mode_${pino}" value="INPUT_PULLUP" data-gpio="${pino}" class="mode-input" />
                                        <span class="label-text">INPUT_PULLUP</span>
                                    </label>
                           
                                    <label class="container-radio">
                                        <input type="radio" name="mode_${pino}" value="INPUT_PULLDOWN" data-gpio="${pino}" class="mode-input" />
                                        <span class="label-text">INPUT_PULLDOWN</span>
                                    </label>
                                </td>
                                <td>
                                    
                                    <label class="container-radio">
                                        <input type="radio" name="level_${pino}" value="1" data-gpio="${pino}" class="level-input" />
                                        <span>HIGH</span>
                                    </label>
                                    <label class="container-radio">
                                        <input type="radio" name="level_${pino}" value="0" data-gpio="${pino}" class="level-input" checked />
                                        <span>LOW</span>
                                    </label>

                                </td>
                            </tr>
                        </tbody>
                    </table>
                </form>
                <span class="border"></span>
                `)
            }).join('');
        }

        // Função Universal de Envio (Fetch API)
        async function enviarComando(url, dados) {
            console.log(`Enviando para ${url}:`, dados);
            
            try {
                const response = await fetch(url, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(dados)
                });

                if (!response.ok) throw new Error('Erro na resposta do servidor');
                
                console.log("Sucesso!");
            } catch (err) {
                console.error("Falha ao enviar comando:", err);
                alert("Erro ao conectar com o dispositivo.");
            }
        }
        document.querySelector('.conteiner-form').addEventListener('change', (event) => {
            console.log(event)
            const target = event.target;
            const pin = target.dataset.gpio;

            if (target.classList.contains('switch-input')) {
                // Liga/Desliga Geral
                enviarComando('/controlar', { pin: parseInt(pin), state: target.checked ? 1 : 0 });
                console.log('seleção')
            } 
            else if (target.classList.contains('mode-input')) {
                // Muda modo (INPUT/OUTPUT)
                console.log(event)
                enviarComando('/config_modo', { pin: parseInt(pin), mode: target.value });
            }
            else if (target.classList.contains('level-input')) {
                // Muda Nível Lógico (HIGH/LOW)
                enviarComando('/set_level', { pin: parseInt(pin), level:  parseInt(target.value) });
            }
        });

        //TESTES DE PINOS
        /*let gpios = {"gpios":{"GPIO0":0,"GPIO2":2}}
        document.querySelector('.conteiner-form').innerHTML = html_form({... gpios.gpios });

        let pinos = [
                {
                    pin: 0,
                    mode: 2,
                    state: 1,
                    level: 0
                },
                {
                    pin: 2,
                    mode: 3,
                    state: 0,
                    level: 1
                }
            ]*/
        let carregarGpios = async () =>  {
            try {
                    // Busca o JSON da rota que você criou no C++
                    let response = await fetch('/config_pinos');
                    let data = await response.json();
           
                    // data.gpios assume que o retorno de meuEsp.pinGPIO() é {"gpios":{...}}
                    let listaGpios = data.gpios; 
                    // Insere o HTML gerado no container
                    document.querySelector('.conteiner-form').innerHTML = html_form(listaGpios);
                    
                } catch (error) {
                    console.error("Erro ao carregar pinos do ESP:", error);
                }
            }
        window.addEventListener('DOMContentLoaded', carregarGpios);

        let verificarStatus = async (event) =>  {
            try {
                let response = await fetch('/status');
                let data = await response.json();
                                console.log("Estados atuais:", data);

                let modelist = [... document.forms];
      
                modelist.forEach((form, i) => {
                    let pin = data[i].pin;
                    let state = data[i].state;
                    let mode = data[i].mode;
                    let level = data[i].level;
                    
                    let switchInput = document.querySelector(`input[name="gpio_${pin}"][data-gpio="${pin}"]`);
                    if (switchInput) switchInput.checked = state;
                    
                    // 2. Sincroniza o Modo (Radio)
                    let modes = mode ===  0 && "INPUT" || mode === 1 && "OUTPUT" || mode === 2 && "INPUT_PULLUP" || mode === 3 && "INPUT_PULLDOWN";
                    let modeInput = form.querySelector(`input[value="${modes}"][data-gpio="${pin}"]`);
                    if (modeInput) modeInput.checked = true;
                    
                    // 3. Sincroniza o Nível Lógico (HIGH/LOW)
                    let levelInput = form.querySelector(`input[value="${level}"][data-gpio="${pin}"]`);
                    console.log(levelInput)
                    if (levelInput) levelInput.checked = true;
                });
               
            } catch (err) {
                console.error("Erro ao ler estados:", err);
            }
        } 
        
        window.addEventListener('DOMContentLoaded', verificarStatus);
    </script>
</body>
</body>
</html>
)rawliteral";

#endif