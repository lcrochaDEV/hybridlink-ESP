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
        @import url("https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200");
        /* Ajuste para tablets e celulares */
        @media (max-width: 768px) {
            .title_header { padding: 15px; }

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
        
        section {
            display: flex;
            justify-content: flex-start;
        }
        /* Remove as bolinhas da lista e alinha os itens */
        .menu-lista {
            list-style: none;
            display: flex;
            gap: 20px;
            padding: 0;
            background-color: var( --primary-color);
            padding: 1rem;
        }

        /* Estilo para os links */
        .menu-lista a {
            text-decoration: none;
            color: #ffffff;
            font-family: sans-serif;
            font-weight: bold;
        }

        /* Estilo específico para a classe MQTT */
        .mqtt {
            color: #0088cc; /* Cor azul comum em tecnologia */
        }

        .mqtt:hover {
            text-decoration: underline;
        }
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
            background: rgba(61, 33, 107, 0.2);
            padding: 5px 8px;
            border-radius: 4px;
            border: 1px solid rgba(255, 255, 255, 0.1);
            font-size: 11px;
            font-weight: bold;
            text-transform: uppercase;
            transition: all 0.2s;
            display: flex;
            align-items: center;
            cursor: pointer;
        }
        .container-radio:hover {
            background: rgba(122, 0, 255, 0.3);
            border-color: #7a00ff;
        }
        .container-radio input:checked + span {
            color: #7a00ff;
            text-shadow: 0 0 5px rgba(0, 255, 213, 0.336);
        }
        /* TABELA*/
        .gpio-table {
            width: 100%%;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            border-radius: 8px;
        }
        .gpio-table tr {
            border-bottom: 1px solid rgba(255, 255, 255, 0.05);
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
        .td-link {
            min-width: 120px;
            padding: 10px;
        }

        .link-select {
            width: 100%%;
            background-color: #1a1a1a;
            color: #00ffd5;
            border: 1px solid #7a00ff;
            border-radius: 4px;
            padding: 5px;
            outline: none;
        }

        /*MQTT DIV*/
        :root {
            --primary-color: #7a00ff;
            --bg-color: #0f172a;
            --card-bg: #1e293b;
            --text-color: #f8fafc;
            --border-color: #334155;
        }
        
        .nexusDialog {padding: 0; background: none; border: none;}

        /* Container principal com title_header e Section lado a lado */
        .config-container { 
            background: var(--card-bg); 
            display: flex; 
            flex-direction: row; 
            border-radius: 12px; 
            box-shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.4); 
            width: 100%%; 
            overflow: hidden;
            border: 1px solid var(--border-color);
        }
        
        /* Ajuste do title_header (Lado Esquerdo) */
        .title_header {
            background-color: var(--primary-color);
            padding: 2.5rem;
            width: 35%%;
            display: flex;
            flex-direction: column;
            justify-content: center;
            max-width: 350px;
        }

        .title_header h2 { margin-top: 0; font-size: 1.5rem; color: var(--text-color);}
        .title_header p {color: var(--text-color);}

        /* Ajuste da Section (Lado Direito) */
        .container_mqtt {
            padding: 2.5rem;
            width: 65%%;
        }
        
        .form-group { margin-bottom: 1.2rem; }
        
        label { display: block; font-weight: 600; margin-bottom: 0.5rem; font-size: 0.9rem; color: #94a3b8; }
        
        input[type="text"], input[type="password"], input[type="number"], select {
            width: 100%%; padding: 10px; border: 1px solid var(--border-color); border-radius: 6px; 
            box-sizing: border-box; font-size: 1rem; background-color: #0f172a; color: white;
        }

        .row { display: grid; grid-template-columns: 3fr 1fr; gap: 10px; }

        .checkbox-group { display: flex; align-items: center; gap: 10px; font-size: 0.9rem; }
        .checkbox-group input { width: auto; }

        .btn-mqtt {
            display: flex;       /* Ativa o modo flexível */
            gap: 10px;           /* Cria um espaço entre os botões */
            align-items: center; /* Alinha verticalmente se tiverem alturas diferentes */
            margin-top: 20px;    
        }
        .btn-save {
            width: 100%%; background-color: var(--primary-color); color: white; border: none; padding: 12px;
            border-radius: 6px; font-weight: bold; cursor: pointer; transition: filter 0.3s;
        }

        .btn-save:hover { filter: brightness(1.2); }
        .btn-close {
            width: 100%%; background-color: #f436b5; color: white; border: none; padding: 12px;
            border-radius: 6px; font-weight: bold; cursor: pointer; transition: filter 0.3s;
            position: relative;
        }

        .security-note { 
            font-size: 0.8rem; color: #64748b; margin-top: 1rem; 
            background: rgba(15, 23, 42, 0.5); padding: 10px; border-left: 4px solid var(--primary-color); 
        }

        /* Responsividade para Mobile */
        @media (max-width: 768px) {
            .config-container { flex-direction: column; }
            .title_header, .container_mqtt { width: 100%%; padding: 1.5rem; }
        }
        .mqtt-config-table {
            width: 100%%;
            border-collapse: collapse;
            margin-top: 30px;
            font-family: sans-serif;
            min-width: 600px;
        }
        .mqtt-config-table th, .mqtt-config-table td, .mqtt-config-table a {
            /*border: 1px solid #64748b;*/
            padding: 8px;
            text-align: left;
            color: #64748b;
        }
        .mqtt-config-table th {border: 1px solid #64748b;}
        .mqtt-config-table th {
            background-color: #7a00ff;
            font-weight: bold;
            color: #fff;
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
<nav>
  <ul class="menu-lista">
    <li><a href="#" class="mqtt">MQTT</a></li>
  </ul>
</nav>
    <section>
        <div class="container_painel coluna">
            <h1>Painel Nexus</h1>
            <h3>%MODULE_VALUE%</h3>
            <div class="conteiner-form"></div>
        </div>
        <!--SENDO CHAMADO POR JS-->

    </section>
    <script>

        document.querySelector('.mqtt').addEventListener('click', async () => {
            try {
                // Busca os dados atuais do ESP
                const response = await fetch('/status');
                const data = await response.json();
                const profiles = data.mqtt || []; // Usando a chave 'mqtt' que definimos no ESP

                // Remove dialog antigo se existir para atualizar os dados
                const oldDialog = document.querySelector('.nexusDialog');
                if (oldDialog) oldDialog.remove();

                document.body.insertAdjacentHTML('beforeend', mqqt_fotm(profiles));
                
                const dialog = document.querySelector('.nexusDialog');
                dialog.addEventListener('click', (e) => {
                    if (e.target === dialog) closeMqttDialog();
                });
                
                dialog.showModal();
            } catch (err) {
                console.error("Erro ao carregar perfis:", err);
            }
        });

        // Função para fechar e remover (limpar memória do navegador)
        function closeMqttDialog() {
            const dialog = document.querySelector('.nexusDialog');
            if (dialog) {
                dialog.close();
                // Opcional: remover do DOM para economizar memória
                // dialog.remove(); 
            }
        }
        // Lógica de salvamento integrada ao seu ecossistema (MQTT/ESP32)
        async function saveMqttConfig() {
            const btn = document.querySelector('.btn-save');
            const formData = {
                broker: document.getElementById('broker')?.value || "",
                port: parseInt(document.getElementById('port')?.value || 1883),
                clientId: document.getElementById('clientId')?.value || "ESP32_Nexus",
                user: document.getElementById('user')?.value || "",
                passw: document.getElementById('passw')?.value || "",
                topic: document.getElementById('baseTopic')?.value || "", 
                ssl: document.getElementById('useSsl')?.checked || false,
                qos: parseInt(document.getElementById('qos')?.value || 0) // 0, 1 ou 2
            };

            btn.innerText = "SINCRONIZANDO...";
            
            try {
                // Envio para o seu backend/ESP32
                const response = await fetch('/config_mqtt', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(formData)
                });

                if(response.ok) {
                    alert("Configuração MQTT atualizada com sucesso!");
                    closeMqttDialog();
                    setTimeout(() => location.reload(), 500); // Dá tempo do ESP processar o LittleFS
                } else if(response.status === 409) {
                    alert("Erro: Este IP e Tópico já estão cadastrados ou limite de 10 perfis atingido.");
                }
            } catch (err) {
                console.error("Falha ao salvar:", err);
            } finally {
                btn.innerText = "Salvar no ESP32";
            }
        }
        async function setActiveMqtt(uuid) {
            try {
                const response = await fetch('/set_active_mqtt', {
                    method: 'PATCH',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ uuid: uuid })
                });

                if (response.ok) {
                    console.log("Perfil ativo alterado!");
                    // Opcional: recarregar o dialog ou apenas desmarcar os outros checkboxes via JS
                    location.reload(); // Recarrega para garantir sincronia total
                } else {
                    alert("Falha ao alterar perfil ativo.");
                }
            } catch (err) {
                console.error("Erro na requisição PATCH:", err);
            }
        }
        let mqqt_fotm = (profiles = []) => {
            // Gera as linhas da tabela dinamicamente
            const tableRows = profiles.map(p => `
                <tr data-uuid="${p.uuid}">
                    <td>${p.clientId || 'N/A'}</td>
                    <td>${p.broker}</td>
                    <td>${p.port}</td>
                    <td>${p.user}</td>
                    <td>${p.topic}</td>
                    <td>${p.ssl ? 'Sim' : 'Não'}</td>
                    <td>${p.qos}</td>
                    <td><input type="checkbox"${p.active ? 'checked' : ''} onchange="setActiveMqtt('${p.uuid}')"></td>
                    <td><a href="javascript:void(0)" onclick="deleteMqtt('${p.uuid}')"><span class="material-symbols-outlined" style="color: #ff4d4d;">delete</span></a></td>
                </tr>
            `).join('');
            return `
                <dialog class="nexusDialog">
                    <div class="config-container">
                        <div class="title_header">
                            <h2>🔒 MQTT</h2>
                            <p style="font-size: 0.9rem; opacity: 0.9;">Configure os parâmetros de conexão para garantir a comunicação segura do seu dispositivo.</p>
                    </div>

                    <div class="container_mqtt">
                        <form id="mqtt-form">
                            <div class="form-group">
                                <label for="broker">Broker (Endereço do Servidor)</label>
                                <div class="row">
                                    <input type="text" id="broker" placeholder="ex: ://meubroker.com">
                                    <input type="number" id="port" placeholder="1883" value="1883">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="clientId">Client ID (Identificador Único)</label>
                                <input type="text" id="clientId" placeholder="ESP32_Sala_01">
                            </div>

                            <div class="form-group">
                                <label for="user">Usuário (Obrigatório para segurança)</label>
                                <input type="text" id="user" placeholder="Digite o usuário">
                            </div>

                            <div class="form-group">
                                <label for="pass">Senha</label>
                                <input type="password" id="passw" placeholder="••••••••">
                            </div>

                            <div class="form-group">
                                <label for="baseTopic">Tópico Base (Prefixo)</label>
                                <input type="text" id="baseTopic" placeholder="minhacasa/dispositivo1/">
                            </div>

                            <div class="form-group checkbox-group">
                                <input type="checkbox" id="useSsl" checked>
                                <label for="useSsl">Usar conexão segura (TLS/SSL)</label>
                            </div>

                            <div class="form-group">
                                <label for="qos">Qualidade de Serviço (QoS)</label>
                                <select id="qos">
                                    <option value="0">0 - No máximo uma vez (Rápido)</option>
                                    <option value="1" selected>1 - Pelo menos uma vez (Confiável)</option>
                                    <option value="2">2 - Exatamente uma vez (Crítico)</option>
                                </select>
                            </div>

                            <div class="btn-mqtt">
                                <button type="button" class="btn-save" onclick="saveMqttConfig()">Salvar Configuração</button>
                                <button class="btn-close" onclick="closeMqttDialog()">CANCELAR</button>
                            </div>
                        </form>

                        <div class="security-note">
                            <strong>Dica de Segurança:</strong> Nunca use o broker sem senha em redes públicas. Para WebSockets (HTML), certifique-se de que o broker suporta conexões <code>wss://</code>.
                        </div>
                        <table class="mqtt-config-table">
                        <thead>
                            <tr>
                                <th>Client</th>
                                <th>Broker</th>
                                <th>Port</th>
                                <th>User</th>
                                <th>Topic</th>
                                <th>SSL</th>
                                <th>QoS</th>
                                <th>Active</th>
                                <th>Ações</th>
                            </tr>
                        </thead>
                        <tbody>
                            ${tableRows || '<tr><td colspan="9">Nenhum perfil cadastrado</td></tr>'}
                        </tbody>
                    </table>
                    </div>
                </dialog>
            `
        }

        let html_form = (list) => {
            // Criamos uma lista de entradas [nome, pino] uma única vez
            const gpiosEntry = Object.entries(list);

            return gpiosEntry.map(([nome, pino]) => {
                
                // FILTRO: Cria as opções excluindo o pino da linha atual
                const optionsHtml = gpiosEntry
                    .filter(([n, p]) => p !== pino) // Remove a si mesmo da lista
                    .map(([n, p]) => `<option value="${p}">${n}</option>`)
                    .join('');

                return (`
                <form id="form_pin_${pino}" class="form-gpio">
                    <table class="gpio-table">
                        <thead>
                            <tr>
                                <th>State</th>
                                <th>Mode</th>
                                <th>Level</th>
                                <th>Vínculo Inteligente</th>
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
                                        <span class="label-text">PULLUP</span>
                                    </label>
                                    <label class="container-radio">
                                        <input type="radio" name="mode_${pino}" value="INPUT_PULLDOWN" data-gpio="${pino}" class="mode-input" />
                                        <span class="label-text">PULLDOWN</span>
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
                                <td class="td-link">
                                    <div class="link-box">
                                        <span style="font-size: 10px; display: block; margin-bottom: 5px;"></span>
                                        <select class="link-select" data-gpio="${pino}">
                                            <option value="-1">NENHUM</option>
                                            ${optionsHtml}
                                        </select>
                                    </div>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </form>
                <span class="border"></span>
                `);
            }).join('');
        }
            

        // Função Universal de Envio (Fetch API)
        async function enviarComando(url, dados) {
            try {
                const response = await fetch(url, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' }, // Corrigido conforme análise
                    body: JSON.stringify(dados)
                });
                console.log(`Resposta de ${url}:`, response.status);
            } catch (err) {
                console.error("Falha na requisição:", err);
            }
        }

        // --- Seu Debounce Manual ---
        let timeoutEnvio;
        const debouncedEnvio = (url, dados) => {
            clearTimeout(timeoutEnvio);
            // Aguarda 100ms de silêncio antes de disparar para o ESP32
            timeoutEnvio = setTimeout(() => enviarComando(url, dados), 100);
        };

        // --- Handler do Switch ---
        function handleSwitch(pin, isChecked) {
            const estado = isChecked ? 1 : 0;
            // Em vez de chamar enviarComando direto, usamos o debounce
            debouncedEnvio('/controlar', { pin: pin, state: estado })
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
            else if(target.classList.contains('link-select')){
                const pinOrigem = parseInt(target.dataset.gpio); // O pino da LINHA
                const pinDestino = parseInt(target.value);      // O pino selecionado no SELECT
                
                // O vínculo só é ativo se o destino for diferente de -1 
                // E, por segurança, diferente do próprio pino de origem
                const active = (pinDestino !== -1 && pinDestino !== pinOrigem);

                enviarComando('/config_link', { 
                    pin: pinOrigem, 
                    target: pinDestino, 
                    active: active 
                });
            }
        });

        //TESTES DE PINOS
        let gpios = {"gpios":{"GPIO0":0,"GPIO2":2}}
        document.querySelector('.conteiner-form').innerHTML = html_form({... gpios.gpios });

        let pinos = [
                {
                    pin: 0,
                    mode: 2,
                    state: 1,
                    level: 0,
                    vincularpin: 2
                },
                {
                    pin: 2,
                    mode: 3,
                    state: 0,
                    level: 1,
                    vincularpin: 0
                }
            ]
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

        let verificarStatus = async (event) => {
            try {
                let response = await fetch('/status');
                let data = await response.json();

                // 1. Condicional: Verifica se há dados no array de pinos
                if (!data.pins || data.pins.length === 0) {
                    console.log("Nenhum pino configurado no sistema.");
                    return; // Encerra aqui se o array estiver vazio []
                }

                // 2. Processamento (Só ocorre se houver itens)
                data.pins.forEach((item) => {
                    if (!item || item.pin === undefined) return;

                    const pin = item.pin;
                    
                    // Sincroniza o Switch (Estado real do GPIO)
                    const switchInput = document.querySelector(`input[name="gpio_${pin}"][data-gpio="${pin}"]`);
                    if (switchInput) switchInput.checked = !!item.state;

                    // Sincroniza o Modo
                    const modesMap = { 0: "INPUT", 1: "OUTPUT", 2: "INPUT_PULLUP", 3: "INPUT_PULLDOWN" };
                    const modeInput = document.querySelector(`input[value="${modesMap[item.mode]}"][data-gpio="${pin}"]`);
                    if (modeInput) modeInput.checked = true;

                    // Sincroniza o Vínculo (Select)
                    const linkSelect = document.querySelector(`.link-select[data-gpio="${pin}"]`);
                    if (linkSelect) {
                        linkSelect.value = (item.vincularpin != null) ? item.vincularpin.toString() : "-1";
                    }
                });

                // 3. Condicional para MQTT (Exemplo se precisar processar)
                if (data.mqtt && data.mqtt.length > 0) {
                    console.log(`Encontrados ${data.mqtt.length} perfis MQTT.`);
                    // Lógica para preencher campos de MQTT aqui...
                }

            } catch (err) {
                console.error("Erro na comunicação com o Nexus:", err);
            }
        }
        
        window.addEventListener('DOMContentLoaded', verificarStatus);

    </script>
</body>
</html>
)rawliteral";

#endif