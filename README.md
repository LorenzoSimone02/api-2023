# Prova finale di Algoritmi e Strutture dati 2022-2023

### **Il codice verrà pubblicato dopo l'ultima verbalizzazione della sessione di Febbraio 2024.**

<p align="justify"> Si consideri un’autostrada descritta come una sequenza di stazioni di servizio. Ogni stazione di servizio si trova
ad una distanza dall’inizio dell’autostrada espressa in chilometri da un numero intero positivo o nullo. Non esistono
due stazioni di servizio aventi la stessa distanza: ogni stazione di servizio è quindi univocamente identificata dalla
sua distanza dall’inizio dell’autostrada.
  
<p align="justify"> Ogni stazione di servizio è dotata di un parco veicoli elettrici a noleggio. Ogni veicolo è contraddistinto
dall’autonomia data da una carica delle batterie, espressa in chilometri, da un numero intero positivo. Il parco
veicoli di una singola stazione comprende al più 512 veicoli. Presa a noleggio un’auto da una stazione s, è possibile
raggiungere tutte quelle la cui distanza da s è minore o uguale all’autonomia dell’automobile. </p>

<p align="justify"> Un viaggio è identificato da una sequenza di stazioni di servizio in cui il conducente fa tappa. Ha quindi inizio
in una stazione di servizio e termina in un’altra, passando per zero o più stazioni intermedie. Si assuma che il
conducente non possa tornare indietro durante il viaggio, e noleggi un’auto nuova ogniqualvolta faccia tappa in
una stazione di servizio. Quindi, date due tappe consecutive s e t, t deve essere sempre più lontana dalla partenza
rispetto a s, e t deve essere raggiungibile usando uno dei veicoli disponibili in s. </p>

<p align="justify"> L’obiettivo del progetto è il seguente: data una coppia di stazioni, pianificare il percorso con il minor numero
di tappe tra di esse. Nel caso siano presenti più percorsi con lo stesso numero minimo di tappe (cioè a pari merito),
deve essere scelto il percorso che predilige le tappe con distanza più breve dall’inizio dell’autostrada. In altre parole,
si consideri l’insieme degli n percorsi a pari merito P = {p<sub>1</sub>, p<sub>2</sub>, . . . p<sub>n</sub>} dove ogni percorso è una tupla di m elementi
pi = ⟨p<sub>i,1</sub>, p<sub>i,2</sub>, . . . p<sub>i,m</sub>⟩ che equivalgono alla distanza da inizio autostrada di ogni tappa in ordine di percorrenza.
Deve essere scelto l’unico percorso p<sub>i</sub> tale che non esista un altro p<sub>j</sub> con le stesse k tappe finali precedute da una
tappa con distanza minore.</p>

<p align="justify">Di seguito un esempio di autostrada. In questo esempio, il percorso corretto tra la stazione a distanza 20 e
quella a distanza 50 è 20 → 30 → 50 (e non 20 → 45 → 50). Si noti che, invece, 50 → 30 → 20 è il percorso
corretto tra la stazione a distanza 50 e quella a distanza 20 (quindi in direzione da destra a sinistra).

<p align="center">
<img width="815" height="135" src="https://github.com/LorenzoSimone02/api-2023/assets/15893018/b616b305-7f4e-470b-9e7c-5195c05b08e6">
</p>

 <br />

## **Formato file in ingresso e stampe attese**
<p align="justify">Il file testuale in ingresso contiene una sequenza di comandi, uno per riga, con il seguente formato. Tutti i valori
interi positivi o nulli sono codificabili in 32 bit.</p>

- **aggiungi-stazione _distanza numero-auto autonomia-auto-1 ... autonomia-auto-n_**
  <p align="justify">Aggiunge una stazione, posta alla distanza indicata, dotata di numero-auto, dall’autonomia indicata.
  Ad esempio:
  aggiungi-stazione 10 3 100 200 300
  aggiunge una stazione a distanza 10 dall’inizio dell’autostrada, con un parco di tre veicoli con autonomia
  100, 200 e 300 km rispettivamente. Se esiste già una stazione alla distanza indicata, il comando non fa nulla.
  Stampa attesa come risposta: aggiunta oppure non aggiunta.</p>


- **demolisci-stazione _distanza_**
  <p align="justify">Rimuove la stazione posta alla distanza indicata, se essa esiste.
  Stampa attesa come risposta: demolita oppure non demolita.</p>


- **aggiungi-auto _distanza-stazione autonomia-auto-da-aggiungere_**
  <p align="justify">Se la stazione esiste, aggiunge un’auto alla stessa. È possibile avere più auto con la stessa autonomia.
  Stampa attesa come risposta: aggiunta oppure non aggiunta.</p>


- **rottama-auto _distanza-stazione autonomia-auto-da-rottamare_**
  <p align="justify">Rimuove un’automobile dalla stazione indicata, se la stazione esiste ed è dotata di almeno un’automobile
  con l’autonomia indicata.
  Stampa attesa come risposta: rottamata oppure non rottamata.</p>


- **pianifica-percorso _distanza-stazione-partenza distanza-stazione-arrivo_**
  <p align="justify">Richiede di pianificare il percorso con i vincoli sopra indicati.
  Stampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazioni
  da inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Devono essere incluse partenza e
  arrivo; se coincidono la stazione viene stampata una sola volta. Se il percorso non esiste, stampa nessun
  percorso. L’azione di pianificazione non altera le stazioni o il loro parco veicoli. Le stazioni date sono
  sicuramente presenti.</p>
