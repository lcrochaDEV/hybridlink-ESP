#ifndef MQTT_HTML_H
#define MQTT_HTML_H

// HTML armazenado na Flash
const char mqtt_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MQTT</title>
    <style>
        @import url("https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200");
        @import url('https://fonts.googleapis.com/css2?family=Vast+Shadow&display=swap');

        /* Ajuste para tablets e celulares */
        @media (max-width: 768px) {
            .title_header { padding: 15px; }


            h1 { font-size: 22px; text-align: center; }
            h3 { text-align: center; width: 100%%; }
        }

        body { 
            margin: 0;
            padding: 0;
            font-family: sans-serif;            
            background: rgba(26, 13, 46, 0.95);
            color: #fff; 
        }
        header { background-color: #3d216b; }
        
        /*section {
            display: flex;
            justify-content: flex-start;
        }*/
        /*MQTT DIV*/
        :root {
            --primary-color: #7a00ff;
            --bg-color: #0f172a;
            --card-bg: #1e293b;
            --text-color: #f8fafc;
            --border-color: #334155;
        }
        
        /* Container principal com title_header e Section lado a lado */
        .config-container { 
            background: var(--card-bg); 
            /*display: flex; 
            flex-direction: row;*/ 
            /*border-radius: 12px;*/ 
            box-shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.4); 
            width: 100%%; 
            overflow: hidden;
            border: 1px solid var(--border-color);
        }

        .module-img {
            display: flex;
            justify-content: start;
            align-items: end;
        }
        .module {
            margin-left: 5px; 
            margin-top: 5px;
        }
        .title_header a {
            color: #f8fafc;
            text-align: center;
            font-size: 45px;
            text-decoration: none;
            font-size: 1.5rem;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            padding: 10px;
        }
        .title_header a:hover {
            background-color: rgba(89, 49, 154, 0.68);
        }
        .title_header img {
            height: 130px;
            margin-right: 10px;
            padding-top: 4px;
            padding-left: 4px;
            border-radius: 15px;
        }
        
        /* Ajuste do title_header (Lado Esquerdo) */
        .title_header {
            background-color: #3d216b;
            display: flex;
            justify-content: flex-start;
        }

        .title_header h2 { margin-top: 0; font-size: 2.5rem; color: var(--text-color); 
            font-family: "Vast Shadow", serif;
            font-weight: 400;
            font-style: normal;
            display: inline-block; /* Obrigatório para o transform funcionar */
            transform: scaleY(2.5);
        }
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
        }style
    </style>
</head>
<body>
    <section>
        <div class="title_header">
            <a href="/"><span class="material-symbols-outlined">arrow_back_ios</span></a>
            <div class="module">
                <div class="module-img">
                    <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcS1J8kCjHTLvpieMjK_ESF8e_vyYlHtsAsUUQ&s" />
                    <div class="blur-container">
                    <h2>MQTT</h2>
                    </div>
                </div>
                <p style="font-size: 0.9rem; opacity: 0.9;">Configure os parâmetros de conexão para garantir a comunicação segura do seu dispositivo.</p>
            </div>
        </div>
        <div class="config-container">

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
            </div>
            <table class="mqtt-config-table">
            </table>
        </div>
    </section>
    <script>
        document.addEventListener('DOMContentLoaded', async () => {
            try {
                //Busca os dados atuais do ESP
                const response = await fetch('/status');
                const data = await response.json();
                const profiles = data.mqtt || []; // Usando a chave 'mqtt' que definimos no ESP

                document.querySelector('.mqtt-config-table').innerHTML += mqqt_fotm(profiles);

            } catch (err) {
                console.error("Erro ao carregar perfis:", err);
            }
        });

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
            // 1. Ativa o perfil no backend
            const response = await fetch('/set_active_mqtt', {
                method: 'PATCH',
                body: JSON.stringify({ uuid })
            });

            if (response.ok) {
                // 2. Opcional: Mostra um estado de "Conectando..." na UI
                updateStatusUI(uuid, "connecting");

                // 3. Aguarda 2 segundos (tempo médio de handshake) e verifica o status real
                setTimeout(async () => {
                    const statusRes = await fetch('/status');
                    const data = await statusRes.json();
                    
                    // 4. Procura o perfil e atualiza a coluna baseado no p.online real do ESP32
                    const profile = data.mqtt.find(m => m.uuid === uuid);
                    updateStatusUI(uuid, profile.online ? "online" : "offline");
                }, 2000);
            }
        }
        async function deleteMqtt(uuid) {
            if (!confirm("Tem certeza que deseja remover este perfil MQTT?")) return;

            try {
                const response = await fetch('/delete_mqtt', {
                    method: 'DELETE',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ uuid: uuid })
                });

                if (response.ok) {
                    console.log("Perfil removido com sucesso");
                    // Remove a linha da tabela visualmente sem precisar dar reload
                    document.querySelector(`tr[data-uuid="${uuid}"]`).remove();
                    
                    // Opcional: Se a tabela ficar vazia, mostrar mensagem
                    const tbody = document.querySelector('.mqtt-config-table tbody');
                    if (tbody.children.length === 0) {
                        tbody.innerHTML = '<tr><td colspan="9">Nenhum perfil cadastrado</td></tr>';
                    }
                } else {
                    alert("Erro ao excluir perfil.");
                }
            } catch (err) {
                console.error("Falha na requisição DELETE:", err);
            }
        }

        function updateStatusUI(uuid, state) {
            const row = document.querySelector(`tr[data-uuid="${uuid}"] .status-col`);
            if (!row) return;

            if (state === "online") {
                row.innerHTML = '<span style="color:#00ff00">● Online</span>';
                document.querySelector('.check [type="checkbox"]').checked = true;
            } else if (state === "connecting") {
                row.innerHTML = '<span style="color:#ffa500">● Conectando...</span>';
            } else {
                row.innerHTML = '<span style="color:#ff4d4d">● Offline</span>';
                document.querySelector('.check [type="checkbox"]').checked = false;
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
                    <td class="check"><input type="checkbox"${p.active ? 'checked' : ''} onchange="setActiveMqtt('${p.uuid}')"></td>
                    <td class="status-col">
                        <span class="status-indicator" style="color: ${p.online ? '#00ff00' : '#ff4d4d'}">
                            ${p.online ? '● Online' : '● Offline'}
                        </span>
                    </td>
                    <td><a href="javascript:void(0)" onclick="deleteMqtt('${p.uuid}')"><span class="material-symbols-outlined" style="color: #ff4d4d;">delete</span></a></td>
                </tr>
            `).join('');
            return `       
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
                        <th>Status</th>
                        <th>Ações</th>
                    </tr>
                </thead>
                <tbody>
                    ${tableRows || '<tr><td colspan="9">Nenhum perfil cadastrado</td></tr>'}
                </tbody>
            `
        }
    </script>
</body>
</html>
)rawliteral";

#endif