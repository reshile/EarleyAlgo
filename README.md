# Алгоритм Эрли

## Сборка

- Сколнируйте репозиторий в директорию Earley

```
> mkdir build
> cd build
> cmake ../Earley
> cmake --build .

```

## Запуск

После сборки в директории build появится исполняемый файл 'main'. 

Формат ввода после запуска:

В первой строке содержатся 3 целых числа 
количество нетерминальных символов, терминальных символов и правил в порождающей грамматике.

Во второй строке - нетерминалы.

В третьей строке - символы алфавита.

В каждой следующей строке записаны правила грамматики в формате:

$$
\begin{align}
N \rightarrow \alpha
\end{align}
$$

$\varepsilon$ в правой части правила обозначается отсутствием правой части (концом строки после ->).

Следующая строка - стартовый символ грамматики.

После нее идет число задающее количесвто слов для проверки. Затем сами слова.


