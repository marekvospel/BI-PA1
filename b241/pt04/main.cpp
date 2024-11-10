#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr int MAX_TRANSPORTS = 100'000;
constexpr int MAX_TRANSPORT_EVENTS = 2 * MAX_TRANSPORTS;

typedef struct TransportEvent {
  long long day;
  long long size;
  long long price;
  long long prefixPrice;
  long long prefixSize;
} TransportEvent;

int parse_transport(TransportEvent transport[], bool first) {
  int scanner = -1;

  if (!first) {
    if (scanf(" }%n", &scanner) == EOF) {
      return 1;
    }
    // } found, not loading this one.
    if (scanner != -1) {
      return -1;
    }
    // we require comma
    scanner = -1;
    if (scanf(" ,%n", &scanner) != 0 || scanner == -1)
      return 1;
  }

  scanner = -1;

  long long from = 0;
  long long to = 0;
  long long size = 0;
  long long price = 0;

  if (scanf(" [ %lld - %lld , %lld , %lld ]%n", &from, &to, &size, &price,
            &scanner) != 4 ||
      scanner == -1) {
    return 1;
  }

  if (from > to || from < 0 || price < 1 || size < 1) {
    return 1;
  }

  transport->day = from;
  transport->size = size;
  transport->price = price;
  transport->prefixPrice = 0;
  transport->prefixSize = 0;

  transport[1].day = to + 1;
  transport[1].size = -size;
  transport[1].price = -price;
  transport[1].prefixPrice = 0;
  transport[1].prefixSize = 0;

  return 0;
}

void print_transports(TransportEvent transports[], size_t length) {
  for (size_t i = 0; i < length; i++) {
    TransportEvent t = transports[i];
    printf("[%lld, %lld, %lld, {%lld, %lld}]", t.day, t.size, t.price,
           t.prefixSize, t.prefixPrice);
  }
}

void merge(TransportEvent out[], TransportEvent left[], size_t leftSize,
           TransportEvent right[], size_t rightSize) {
  size_t leftIndex = 0;
  size_t rightIndex = 0;
  for (; leftIndex < leftSize && rightIndex < rightSize;) {
    if (left[leftIndex].day > right[rightIndex].day) {
      out[leftIndex + rightIndex] = right[rightIndex];
      rightIndex++;
    } else {
      out[leftIndex + rightIndex] = left[leftIndex];
      leftIndex++;
    }
  }

  while (leftIndex < leftSize) {
    out[leftIndex + rightIndex] = left[leftIndex];
    leftIndex++;
  }
  while (rightIndex < rightSize) {
    out[leftIndex + rightIndex] = right[rightIndex];
    rightIndex++;
  }
}

void merge_sort(TransportEvent array[], size_t length) {
  if (length <= 1)
    return;

  TransportEvent *work =
      (TransportEvent *)malloc(length * sizeof(TransportEvent));
  size_t rightSize = length / 2;
  size_t leftSize = length - rightSize;
  if (work == 0) {
    printf("Allocation failed :(\n");
    abort();
  }
  memcpy(work, array, sizeof(TransportEvent) * length);
  // Left and right creaed

  merge_sort(work, leftSize);
  merge_sort(work + leftSize, rightSize);

  merge(array, work, leftSize, work + leftSize, rightSize);

  free(work);
}

void normalize_values(TransportEvent array[], size_t length) {
  long long currentSize = 0;
  long long currentPrice = 0;
  long long totalSize = 0;
  long long totalPrice = 0;

  for (size_t i = 0; i < length; i++) {
    if (i != 0) {
      long long dayDiff = (array[i].day - array[i - 1].day);
      totalSize += dayDiff * currentSize;
      totalPrice += dayDiff * currentPrice;
    }

    for (size_t y = i; y < length; y++) {
      if (array[y].day != array[i].day)
        break;

      currentSize += array[y].size;
      currentPrice += array[y].price;
    }

    for (size_t y = i; y < length; y++) {
      if (array[y].day != array[i].day)
        break;
      if (y != i)
        i += 1;
      array[y].size = currentSize;
      array[y].price = currentPrice;
      array[y].prefixSize = totalSize;
      array[y].prefixPrice = totalPrice;
    }
  }
}

size_t binary_search_start(TransportEvent work[], size_t transportLen,
                           long long startDay) {
  if (transportLen == 1)
    return 0;
  size_t checkIndex = transportLen / 2;

  if (work[checkIndex].day >= startDay &&
      (checkIndex - 1 < 0 || work[checkIndex - 1].day < startDay))
    return checkIndex;

  if (work[checkIndex].day < startDay) {
    int result = binary_search_start(work + checkIndex,
                                     transportLen - checkIndex, startDay);
    return result == -1 ? -1 : checkIndex + result;
  } else {
    return binary_search_start(work, checkIndex, startDay);
  }
}

size_t binary_search_end(TransportEvent work[], size_t transportLen,
                         long long sizeNeeded, long long prefixSize) {
  if (transportLen == 1)
    return 0;
  size_t checkIndex = transportLen / 2;

  if ((work[checkIndex].prefixSize - prefixSize) < sizeNeeded &&
      (checkIndex - 1 < 0 ||
       (work[checkIndex - 1].prefixSize - prefixSize) >= sizeNeeded))
    return checkIndex;

  if ((work[checkIndex].prefixSize - prefixSize) < sizeNeeded) {
    size_t result = binary_search_end(
        work + checkIndex, transportLen - checkIndex, sizeNeeded, prefixSize);
    return result == (size_t)-1 ? -1 : checkIndex + result;
  } else {
    return binary_search_end(work, checkIndex, sizeNeeded, prefixSize);
  }
}

int calculate(TransportEvent work[], size_t transport_len, long long day,
              long long size) {
  long long endDay = 0;
  long long totalPrice = 0;
  long long remaining = 0;

  size_t start_index = binary_search_start(work, transport_len, day);
  if (start_index == (size_t)-1)
    return -1;

  if (start_index > 0) {
    long long days = work[start_index].day - day;
    size -= work[start_index - 1].size * days;
    totalPrice += work[start_index - 1].price * days;

    if (size < 0) {
      long long days = -size / work[start_index - 1].size;
      endDay = work[start_index].day - days - 1;
      totalPrice -= days * work[start_index - 1].price;
      printf("Konec: %lld, cena: %lld\n", endDay, totalPrice);
      return 0;
    }
  }

  size_t end_index = binary_search_end(work, transport_len, size,
                                       work[start_index].prefixSize);
  if (end_index == (size_t)-1)
    return -1;

  // + extra calculated after
  endDay = work[end_index].day - 1;
  totalPrice += work[end_index].prefixPrice - work[start_index].prefixPrice;
  remaining =
      size - (work[end_index].prefixSize - work[start_index].prefixSize);

  // remaining days from now
  if (remaining > 0 && work[end_index].size != 0) {
    remaining = remaining / work[end_index].size +
                (remaining % work[end_index].size != 0);
    endDay += remaining;
    totalPrice += work[end_index].price * remaining;
  } else if (remaining > 0) {
    return -1;
  }

  printf("Konec: %lld, cena: %lld\n", endDay, totalPrice);
  return 0;
}

int parse_transport(TransportEvent transport[], size_t *transport_len) {
  printf("Moznosti dopravy:\n");
  int scanner = -1;

  if (scanf(" {%n", &scanner) != 0 || scanner == -1) {
    return -1;
  }

  for (size_t i = 0; i <= MAX_TRANSPORT_EVENTS; i += 2) {
    int loaded = parse_transport(&transport[i], i == 0);
    if (loaded < 0) {
      *transport_len = i;
      if (*transport_len == 0)
        return -1;
      break;
    } else if (loaded > 0 || i == MAX_TRANSPORT_EVENTS) {
      return -1;
    }
  }
  return 0;
}

int main() {
  TransportEvent *transport =
      (TransportEvent *)malloc(MAX_TRANSPORT_EVENTS * sizeof(TransportEvent));
  size_t transportLen = 0;
  if (transport == 0) {
    return -1;
  }

  if (parse_transport(transport, &transportLen) != 0)
    goto invalid_input;

  merge_sort(transport, transportLen);
  normalize_values(transport, transportLen);

  printf("Naklad:\n");

  while (true) {
    long long day;
    int scanner = -1;
    long long size;
    int result = scanf(" %lld%n %lld", &day, &scanner, &size);
    if (result != 2) {
      if (scanner != -1 || result != EOF)
        goto invalid_input;
      break;
    }
    if (day < 0 || size <= 0)
      goto invalid_input;

    result = calculate(transport, transportLen, day, size);
    if (result != 0) {
      printf("Prilis velky naklad, nelze odvezt.\n");
    }
  }

  free(transport);
  return 0;
invalid_input:
  printf("Nespravny vstup.\n");
  free(transport);
  return -1;
}
