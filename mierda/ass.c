// XXX: http://codeforces.com/problemset/problem/220/E
#if 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define CACA_COMUN_IDX_INVALIDO ((natural)CACA_COMUN_VALOR_INVALIDO)
//#define CACA_COMUN_LOG

typedef unsigned int natural;
typedef int tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;
typedef char byteme;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_NIMADRES
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){while(1){printf("");};abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

#define caca_comun_calloc_local(tipo) (&(tipo){0})

void caca_log_debug_func(const char *format, ...);

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#ifdef CACA_COMUN_LOG
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%12d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}

static char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%2I64d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
#else
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	return NULL;
}

static char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	return NULL;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas,
		int *num_columnas, int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

static inline char caca_comun_letra_a_valor_minuscula(char letra) {
	return letra - 'a';
}

static inline natural caca_comun_max_natural(natural *nums, natural nums_tam) {
	natural max = 0;

	for (int i = 0; i < nums_tam; i++) {
		natural num_act = nums[i];
		if (num_act > max) {
			max = num_act;
		}
	}

	return max;
}

// XXX: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
static inline char *caca_comun_trimea(char *cad, natural cad_tam) {
	char c = cad[0];
	char tmp = '\0';
	natural i = 0;
	natural j = 0;

	caca_log_debug("entrada %s cad_tam %u", cad, cad_tam);
	while (j < cad_tam && cad[j] != '\0') {
		caca_log_debug("en j %u car %c", j, cad[j]);
		while (j < cad_tam && !isalpha(cad[j])) {
			caca_log_debug("brincando j %u car %c", j, cad[j]);
			j++;
		}
		caca_log_debug("aora j %u car %c", j, cad[j]);
		if (j == cad_tam) {
			caca_log_debug("q ped");
			break;
		}
		tmp = cad[i];
		cad[i] = cad[j];
		cad[j] = tmp;
		i++;
		j++;
	}
	caca_log_debug("mierda '%c'", cad[j]);

	i = 0;
	while (isalpha(cad[i++]))
		;
	caca_log_debug("salida %s", cad);
	cad[i - 1] = '\0';

	return cad;
}

#endif

#endif

#if 1
typedef struct numero_orden {
	int numero;
	natural posicion_inicial;
} numero_orden;

static int compara_numero_orden(const void *pa, const void *pb) {
	numero_orden *a = (numero_orden *) pa;
	numero_orden *b = (numero_orden *) pb;
	int r = 0;

	r = a->numero - b->numero;

	if (!r) {
		r = a->posicion_inicial - b->posicion_inicial;
	}

	assert(r);

	return r;
}

static inline natural *mapear_a_posicion_ordenada(natural *nums,
		natural nums_tam, natural *nums_res, natural nums_res_tam) {
	numero_orden *nums_ord = NULL;

	assert(nums_res_tam >= nums_tam);

	nums_ord = calloc(nums_tam, sizeof(numero_orden));
	assert(nums_ord);

	for (natural i = 0; i < nums_tam; i++) {
		numero_orden *num_ord_act = nums_ord + i;

		num_ord_act->numero = nums[i];
		num_ord_act->posicion_inicial = i;
	}

	qsort(nums_ord, nums_tam, sizeof(numero_orden), compara_numero_orden);

	for (natural i = 0; i < nums_tam; i++) {
		numero_orden *num_ord_act = nums_ord + i;
		nums_res[num_ord_act->posicion_inicial] = i;
	}

	free(nums_ord);
	return nums_res;
}
#endif

#if 1

#define BIT_CH_VALOR_INVALIDO ULLONG_MAX

typedef struct bit_ch {
	natural num_nodos_bit_ch;
	tipo_dato *nodos_bit_ch;
} bit_ch;

static inline void bit_ch_aumenta(bit_ch *bit, tipo_dato nuevo_valor,
		natural idx);

static inline void bit_ch_init(bit_ch *bit, tipo_dato valor_inicial,
		natural num_cacas) {
	tipo_dato *nodos = NULL;

	bit->num_nodos_bit_ch = num_cacas + 2;

	bit->nodos_bit_ch = calloc(bit->num_nodos_bit_ch, sizeof(tipo_dato));
	nodos = bit->nodos_bit_ch;

	nodos[0] = (tipo_dato) BIT_CH_VALOR_INVALIDO;
	for (natural i = 1; i < bit->num_nodos_bit_ch; i++) {
		nodos[i] = 0;
	}

	bit_ch_aumenta(bit, valor_inicial, 1);
}

static inline void bit_ch_fini(bit_ch *bit) {
	free(bit->nodos_bit_ch);
}

static inline void bit_ch_aumenta(bit_ch *bit, tipo_dato nuevo_valor,
		natural idx) {
	tipo_dato *nodos = bit->nodos_bit_ch;
	caca_log_debug("q verga actualizando %u con %d", idx, nuevo_valor);
	for (natural i = idx; i && i <= bit->num_nodos_bit_ch; i += (i & (-i))) {
		caca_log_debug("actualizando caca %u con %d antes %d", i, nuevo_valor,
				nodos[i]);
		nodos[i] += nuevo_valor;
		caca_log_debug("actualizado caca %u aora %d", i, nodos[i]);
	}
}

static inline tipo_dato bit_ch_consulta(bit_ch *bit, natural idx) {
	tipo_dato *nodos = bit->nodos_bit_ch;
	tipo_dato res = 0;
	for (int i = idx; i > 0; i -= (i & (-i))) {
		assert_timeout(i < bit->num_nodos_bit_ch);
		caca_log_debug("consultando caca %u tiene %u", i, nodos[i]);
		res += nodos[i];
	}
	caca_log_debug("regresando %d", res);
	return res;
}

static inline tipo_dato bit_ch_consulta_rango_actualizacion_puntual(bit_ch *bit,
		natural i, natural j) {
	return bit_ch_consulta(bit, j) - bit_ch_consulta(bit, i - 1);
}

static inline tipo_dato bit_ch_consulta_fondo(bit_ch *bit, natural i) {
	return bit_ch_consulta_rango_actualizacion_puntual(bit, i,
			bit->num_nodos_bit_ch - 1);
}

static inline void bit_ch_aumenta_rango(bit_ch *bit, tipo_dato nuevo_valor,
		natural idx_ini, natural idx_fin) {
	caca_log_debug("aumentando de %u a %u con valor %I64d", idx_ini, idx_fin,
			nuevo_valor);

	bit_ch_aumenta(bit, nuevo_valor, idx_ini);
	bit_ch_aumenta(bit, -nuevo_valor, idx_fin + 1);
}

static inline void bit_ch_aumenta_rango_consulta_rango(bit_ch *bit_puto,
		bit_ch *bit_aux, natural idx_ini, natural idx_fin,
		tipo_dato nuevo_valor) {

	caca_log_debug("aumentando de %u a %u con valor %I64d para consulta rango",
			idx_ini, idx_fin, nuevo_valor);

	bit_ch_aumenta_rango(bit_puto, nuevo_valor, idx_ini, idx_fin);
	caca_log_debug(
			"actualizando para consulta rango inicial %d con %I64d (%I64d * %u)",
			idx_ini, nuevo_valor * (idx_ini - 1), nuevo_valor, idx_ini - 1);
	bit_ch_aumenta(bit_aux, nuevo_valor * (idx_ini - 1), idx_ini);
	caca_log_debug(
			"actualizando para consulta rango final %d con %I64d(%I64d,%u)",
			idx_fin + 1, -nuevo_valor * idx_fin, -nuevo_valor, idx_fin);
	bit_ch_aumenta(bit_aux, -nuevo_valor * idx_fin, idx_fin + 1);
}

static inline tipo_dato bit_ch_consulta_rango_actualizado_rango(
		bit_ch *bit_puto, bit_ch *bit_aux, natural idx) {
	tipo_dato resul = 0;
	tipo_dato valor_puto = 0;
	tipo_dato valor_aux = 0;

	valor_puto = bit_ch_consulta(bit_puto, idx);

	caca_log_debug(
			"el valor putual en %u es %I64d, aportara %I64d (%I64d * %u)", idx,
			valor_puto, valor_puto * idx, valor_puto, idx);

	valor_aux = bit_ch_consulta(bit_aux, idx);

	caca_log_debug("el valor aux en %u es %I64d", idx, valor_puto);

	resul = valor_puto * idx - valor_aux;

	caca_log_debug("la suma acumulacaca hasta %u es %I64d", idx, resul);

	return resul;
}

static inline tipo_dato bit_ch_consulta_rango(bit_ch *bit_puto, bit_ch *bit_aux,
		natural idx_ini, natural idx_fin) {
	tipo_dato resul = 0;
	tipo_dato resul_idx_ini = 0;
	tipo_dato resul_idx_fin = 0;

	resul_idx_ini = bit_ch_consulta_rango_actualizado_rango(bit_puto, bit_aux,
			idx_ini - 1);
	caca_log_debug("la suma acumulacaca ini hasta %u es %I64d", idx_ini,
			resul_idx_ini);

	resul_idx_fin = bit_ch_consulta_rango_actualizado_rango(bit_puto, bit_aux,
			idx_fin);
	caca_log_debug("la suma acumulacaca fin hasta %u es %I64d", idx_fin,
			resul_idx_fin);

	resul = resul_idx_fin - resul_idx_ini;

	caca_log_debug("la suma acumulacaca del rango %u-%u es %I64d", idx_ini,
			idx_fin, resul);

	return resul;
}

#endif

static inline natural modifica(bit_ch *bi, bit_ch *bd, natural *a, natural i,
		bool quitar) {
	natural inv = 0;
	bit_ch *bm = NULL;
	int factor = 0;
	inv = bit_ch_consulta_fondo(bi, a[i] + 1) + bit_ch_consulta(bd, a[i] - 1);

	if (quitar) {
		bm = bd;
		factor = -1;
	} else {
		bm = bi;
		factor = 1;
	}
	caca_log_debug("el puto factor %d", factor);

	bit_ch_aumenta(bm, factor, a[i]);
	caca_log_debug("las inv al %s %u son %u", quitar?"quitar":"poner", a[i],
			inv);

	return inv;
}

static inline natural quita(bit_ch *bi, bit_ch *bd, natural *a, natural i) {
	return modifica(bi, bd, a, i, verdadero);
}

static inline natural anade(bit_ch *bi, bit_ch *bd, natural *a, natural i) {
	return modifica(bi, bd, a, i, falso);
}

static inline entero_largo_sin_signo core(natural *nums, natural nums_tam,
		entero_largo_sin_signo k) {
	entero_largo_sin_signo inv = 0;
	bit_ch *bi = caca_comun_calloc_local(bit_ch);
	bit_ch *bd = caca_comun_calloc_local(bit_ch);
	natural j = 0;
	entero_largo_sin_signo r = 0;
	natural *a = NULL;

	a = calloc(nums_tam, sizeof(natural));
	assert_timeout(a);

	mapear_a_posicion_ordenada(nums, nums_tam, a, nums_tam);
	for (natural i = 0; i < nums_tam; i++) {
		a[i]++;
	}
	caca_log_debug("mapeados %s",
			caca_comun_arreglo_a_cadena_natural(a, nums_tam, CACA_COMUN_BUF_STATICO));

	bit_ch_init(bi, 0, nums_tam);
	bit_ch_init(bd, 0, nums_tam);

	for (natural i = 0; i < nums_tam; i++) {
		caca_log_debug("anadiendo %u en pos %u", a[i], i);
		inv += bit_ch_consulta_fondo(bd, a[i]);
		bit_ch_aumenta(bd, 1, a[i]);
	}

	caca_log_debug("inv totales %I64d", inv);

	anade(bi, bd, a, 0);
	for (natural i = 1; i < nums_tam; i++) {
		while (j < nums_tam && (j < i || inv > k)) {
			inv -= quita(bi, bd, a, j);
			j++;
			caca_log_debug("kitando %u en pos %u inv %I64d", a[j-1], j-1, inv);
		}

		r += nums_tam - j;

		caca_log_debug("en %u %u inv %I64d r %I64d", i, j, inv, r)

		inv += anade(bi, bd, a, i);
	}

	bit_ch_fini(bi);
	bit_ch_fini(bd);

	return r;
}

#define MAX_CACA ((int)1E5)
natural a[MAX_CACA] = { 0 };

int main() {
	natural n = 0;
	entero_largo_sin_signo k = 0;

	scanf("%u %llu\n", &n, &k);
	assert_timeout(n<=MAX_CACA);
	assert_timeout(k < ((entero_largo_sin_signo )(1E18)));

	for (natural i = 0; i < n; i++) {
		scanf("%u", &a[i]);
	}

	printf("%llu\n", core(a, n, k));
	return EXIT_SUCCESS;
}
