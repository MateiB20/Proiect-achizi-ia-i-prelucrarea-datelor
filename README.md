 
Universitatea Tehnică “Gheorghe Asachi” Iași
Facultatea Automatică și Calculatoare
Specializarea Calculatoare și Tehnologia Informației
Disciplina: Achiziția si prelucrarea datelor-proiect

Procesarea semnalului, analiza datelor și analiza spectrală

Student: Bărîlă Matei  		
Grupa:1310A

Rezumatul proiectului:
Se realizează interfața în LabWindows/CVI pentru încărcare unui fișier wav și se afișează primele 6 secunde ale semnalului pe un control de tip Graph. 
Se mai afișează atât valoarea minimă și cea maximă cât și indexul acestora, dispersia, media, mediana și numărul de treceri prin zero, toate pe  controale de tip Numeric, și histograma pe un alt control de tip Graph. Pentru un interval de timp ce conține primele 256 de eșantioane se calculează și afișează skewness (asimetria care caracterizează forma repartiției) și kurtosis (aplatizarea care pune în evidență vârful distribuției de probabilitate). 
Se realizează afișarea semnalului filtrat în domeniul timp prin filtrare prin mediere sau cu element de ordinul I și a anvelopei semnalului pe același control Graph deja utilizat pentru semnal. 
Pe interfața aplicației este creat un control pentru selecția tipului de filtru și respectiv Prev și Next care realizează trecerea la secunda anterioară sau la cea următoare. Pentru filtrul de ordin I, valoarea parametrului alpha se fixează prin intermediul unui control numeric în intervalul (0, 1). 
În alt panou se includ controalele necesare prelucrării în frecvență: un control de tip Graph pentru reprezentarea spectrului pe o anume fereastră de timp, un control numeric tip întreg pentru selectarea numărului de puncte (mai exact dimensiunea ferestrei de timp) pentru calculul FFT și un Timer. 
La fiecare tick al Timer-ului se calculează spectrul și se reprezintă grafic. Graficul se salvează ca fișier jpg.  Se reprezintă semnalul și spectrul înainte și după filtrare și ferestruire (Chebyshev de ordin I/II, trece banda 900 - 1100 Hz respectiv hanning/hamming).
Cerințele proiectului:
Utilizatorul va vizualiza primele 6 secunde ale semnalului pe un control grafic, împreună cu statistici și va oferi opțiuni pentru aplicarea unor filtre sau ferestre și vizualizarea derivatei semnalului filtrat. Utilizatorul va avea posibilitatea de a alege tipul de filtru și de a naviga între secunde. În analiza frecvenței graficele vor fi actualizate periodic printr-un Timer. Toate graficele vor fi salvate automat în format jpg. 

Fișierul utilizat:
Fișierul utilizat în cadrul acestui proiect este un fișier audio în format WAV (Waveform Audio File Format) care este utilizat pentru a salva datele audio fără pierderi de informație. Semnalul audio din fișierul WAV este reprezentat printr-o secvență de eșantioane, fiecare având o valoare numerică corespunzătoare amplitudinii semnalului într-un anumit moment de timp.

Mediul de dezvoltare:
Aplicația este realizată în LabWindows/CVI, un mediu de dezvoltare integrat (IDE) creat de National Instruments care este destinat în mod particular pentru aplicații pentru prelucrarea semnalelor, automatizarea testelor și interacțiunea cu echipamente de laborator. 
Un script Python utilizează numpy și scipy, mai întâi pentru ca fișierul WAV să fie citit și pentru a obține frecvența de eșantionare și datele audio. Se extrage un segment de 6 secunde din semnalul audio, iar informațiile despre fișier și datele segmentului audio și în plus, anvelopa semnalului sunt salvate în fișiere text.

Detalii de implementare și comentarii privind rezultatele obținute:
Etapa 1:
În etapa 1 am folosit MaxMin1D pentru valorile maxime și minime din vectorul de intrare și locațiile acestora în vector. Dacă există mai multe instanțe ale valorilor maxime și/sau minime, MaxMin1D returnează locația primei apariții. Median găsește valoarea mediană a intrării. 
Cu countZeroCrossings calculez numărul de treceri prin zero. O trecere prin zero reprezintă momentul în care un semnal trece de la o valoare pozitivă la una negativă sau invers.


Pentru calcularea și reprezentarea histogramei se împarte intervalul valorilor din waveData în bins, iar fiecare valoare din datele de intrare este plasată într-unul dintre aceste bin-uri, în funcție de valoarea sa. Histograma finală este histogram, un vector de dimensiune nbins, iar fiecare element din acest vector reprezintă numărul de elemente care au fost plasate în bin-ul respectiv.
 
Histogramă întreg semnal
ApplyMovingAverageFilter aplică un filtru de mediere pe un semnal, în vederea netezirii acestuia. Aici start este începutul ferestrei de mediere, iar end este sfârșitul. Se face suma valorilor din semnalul de intrare care se află în intervalul definit de [start, end]. După aceea, se împarte suma la numărul de elemente din fereastră pentru a obține media. Media calculată este salvată în vectorul de ieșire filt pentru elementul i respectiv.
 
ApplyFirstOrderFilter aplică un filtru IIR de ordin 1 pe semnalul de intrare, utilizând o formulă recursivă de tipul: 
filt[i]=(1-alpha)*filt[i-1]+alpha*signal[i]
 

Asimetria și aplatizarea se află cu funcția Moment pentru un interval de timp ce conține primele 256 de eșantioane. Moment calculează momentul pentru media vectorului de intrare cu ordinea specificată. Moment folosește următoarele formule pentru a găsi momentul:
 
Anvelopa semnalului se obține cu transformata Hilbert printr-o secvență Python.
 
DerivativeForward calculează derivata aproximativă a unui semnal folosind metoda diferenței înainte (forward difference). Funcția calculează diferența înainte între elementele consecutive ale semnalului. Derivata aproximativă la punctul i este calculată ca diferența între valoarea semnalului la punctul i+1 și valoarea semnalului la punctul i. Această metodă de aproximare a derivatei este folosită pentru a obține o estimare a ratei de schimbare a semnalului între punctele adiacente.
 
Etapa 2:
ApplyHammingWindow aplică o fereastră Hamming pe semnal pentru a reduce efectele de împrăștiere a energiei atunci când se face transformata Fourier a semnalului.
 
ApplyHanningWindow aplică o fereastră Hanning pe semnal, care este utilizată pentru a atenua efectele de împrăștiere a energiei și a obține o mai bună reprezentare spectrală.
ApplyChebyshevI_BandPassFilter aplică un filtru Chebyshev de ordin I pe semnal, care este un filtru de tip band-pass care permite o distorsiune de amplitudine în banda de trecere.
ApplyChebyshevII_BandPassFilter aplică un filtru Chebyshev de ordin II pe semnal, care, spre deosebire de ordinul I, garantează o răspuns mai plat în banda de trecere.

Obținem spectrul prin: ScaledWindowEx, AutoPowerSpectrum, PowerFrequencyEstimate și SpectrumUnitConversion. Scopul acestor funcții este de a analiza un semnal pentru a obține informații detaliate despre distribuția sa spectrală pentru: Identificarea frecvențelor dominante ale semnalului, măsurarea puterii semnalului pe diferite frecvențe și conversia și interpretarea rezultatelor într-un format util pentru analize ulterioare.
 
