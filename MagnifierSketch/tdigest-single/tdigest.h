#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>


#define M_PI 3.14159265358979323846

typedef struct node {
     double mean;
     double count;
} node_t;

struct td_histogram {
     // compression is a setting used to configure the size of centroids when merged.
     double compression; 
     
     // cap is the total size of nodes
     int cap;
     // merged_nodes is the number of merged nodes at the front of nodes.
     int merged_nodes;
     // unmerged_nodes is the number of buffered nodes.
     int unmerged_nodes;
     
     double merged_count;
     double unmerged_count;
     
     node_t nodes[0];
};

void td_add(td_histogram *h, double mean, double count);

static bool is_very_small(double val) {
     return !(val > .000000001 || val < -.000000001);
}

static int cap_from_compression(double compression) {
     return (6 * (int)(compression)) + 10;
}

static bool should_merge(td_histogram *h) {
     return ((h->merged_nodes + h->unmerged_nodes) == h->cap);
}

static int next_node(td_histogram *h) {
     return h->merged_nodes + h->unmerged_nodes;
}

static void merge(td_histogram *h);

////////////////////////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////////////////////////



static size_t td_required_buf_size(double compression) {
     return sizeof(td_histogram) +
          (cap_from_compression(compression) * sizeof(node_t));
}

// td_init will initialize a td_histogram inside buf which is buf_size bytes.
// If buf_size is too small (smaller than compression + 1) or buf is NULL,
// the returned pointer will be NULL.
//
// In general use td_required_buf_size to figure out what size buffer to
// pass.
static td_histogram *td_init(double compression, size_t buf_size, char *buf) {
     td_histogram *h = (td_histogram *)(buf);
     if (!h) {
          return NULL;
     }
     bzero((void *)(h), buf_size);
     *h = (td_histogram) {
          .compression = compression,
          .cap = static_cast<int>((buf_size - sizeof(td_histogram)) / sizeof(node_t)),
          .merged_nodes = 0,
          .unmerged_nodes = 0,
          .merged_count = 0,
          .unmerged_count = 0,
     };
     return h;
}

td_histogram *td_new(double compression) {
     size_t memsize = td_required_buf_size(compression);
     return td_init(compression, memsize, (char *)(malloc(memsize)));
}

void td_free(td_histogram *h) {
     free((void *)(h));
}

void td_merge(td_histogram *into, td_histogram *from) {
     merge(into);
     merge(from);
     for (int i = 0; i < from->merged_nodes; i++) {
          node_t *n = &from->nodes[i];
          td_add(into, n->mean, n->count);
     }
}

void td_reset(td_histogram *h) {
     bzero((void *)(&h->nodes[0]), sizeof(node_t)*h->cap);
     h->merged_nodes = 0;
     h->merged_count = 0;
     h->unmerged_nodes = 0;
     h->unmerged_count = 0;
}

void td_decay(td_histogram *h, double factor) {
     merge(h);
     h->unmerged_count *= factor;
     h->merged_count *= factor;
     for (int i = 0; i < h->merged_nodes; i++) {
          h->nodes[i].count *= factor;
     }
}

double td_total_count(td_histogram *h) {
     return h->merged_count + h->unmerged_count;
}

double td_total_sum(td_histogram *h) {
     node_t *n = NULL;     
     double sum = 0;
     int total_nodes = h->merged_nodes + h->unmerged_nodes;
     for (int i = 0; i < total_nodes; i++) {
          n = &h->nodes[i];
          sum += n->mean * n->count;
     }
     return sum;
}

double td_quantile_of(td_histogram *h, double val) {
     merge(h);
     if (h->merged_nodes == 0) {
          return NAN;
     }
     double k = 0;
     int i = 0;
     node_t *n = NULL;
     for (i = 0; i < h->merged_nodes; i++) {
          n = &h->nodes[i];
          if (n->mean >= val) {
               break;
          }
          k += n->count;
     }
     if (val == n->mean) {
          // technically this needs to find all of the nodes which contain this value and sum their weight
          double count_at_value = n->count;
          for (i += 1; i < h->merged_nodes && h->nodes[i].mean == n->mean; i++) {
               count_at_value += h->nodes[i].count;
          }
          return (k + (count_at_value/2)) / h->merged_count;
     } else if (val > n->mean) { // past the largest
          return 1;
     } else if (i == 0) {
          return 0;
     }
     // we want to figure out where along the line from the prev node to this node, the value falls
     node_t *nr = n;
     node_t *nl = n-1;
     k -= (nl->count/2);
     // we say that at zero we're at nl->mean
     // and at (nl->count/2 + nr->count/2) we're at nr
     double m = (nr->mean - nl->mean) / (nl->count/2 + nr->count/2);
     double x = (val - nl->mean) / m;
     return (k + x) / h->merged_count;
}


double td_value_at(td_histogram *h, double q) {
     merge(h);
     if (q < 0 || q > 1 || h->merged_nodes == 0) {
          return NAN;
     }
     // if left of the first node, use the first node
     // if right of the last node, use the last node, use it
     double goal = q * h->merged_count;
     double k = 0;
     int i = 0;
     node_t *n = NULL;
     for (i = 0; i < h->merged_nodes; i++) {
          n = &h->nodes[i];
          if (k + n->count > goal) {
               break;
          }
          k += n->count;
     }
     double delta_k = goal - k - (n->count/2);
     if (is_very_small(delta_k)) {
          return n->mean;
     }
     bool right = delta_k > 0;
     if ((right && ((i+1) == h->merged_nodes)) ||
         (!right && (i == 0))) {
          return n->mean;
     }
     node_t *nl;
     node_t *nr;
     if (right) {
          nl = n;
          nr = &h->nodes[i+1];
          k += (nl->count/2);
     } else {
          nl = &h->nodes[i-1];
          nr = n;
          k -= (nl->count/2);
     }
     double x = goal - k;
     // we have two points (0, nl->mean), (nr->count, nr->mean)
     // and we want x
     double m = (nr->mean - nl->mean) / (nl->count/2 + nr->count/2);
     return m * x + nl->mean;
}

double td_trimmed_mean(td_histogram *h, double lo, double hi) {
     if (should_merge(h)) {
          merge(h);
     }
     double total_count = h->merged_count;
     double left_tail_count = lo * total_count;
     double right_tail_count = hi * total_count;
     double count_seen = 0;
     double weighted_mean = 0;
     for (int i = 0; i < h->merged_nodes; i++) {
          if (i > 0) {
               count_seen += h->nodes[i-1].count;
          }
          node_t *n = &h->nodes[i];
          if (n->count < left_tail_count) {
               continue;
          }
          if (count_seen > right_tail_count) {
               break;
          }
          double left = count_seen;
          if (left < left_tail_count) {
               left = left_tail_count;
          }
          double right = count_seen + n->count;
          if (right > right_tail_count) {
               right = right_tail_count;
          }
          weighted_mean += n->mean * (right - left);          
     }
     double included_count = total_count * (hi - lo);
     return weighted_mean / included_count;
}

void td_add(td_histogram *h, double mean, double count) {
     if (should_merge(h)) {
          merge(h);
     }
     h->nodes[next_node(h)] = (node_t) {
          .mean = mean,
          .count = count,
     };
     h->unmerged_nodes++;
     h->unmerged_count += count;
}

static int compare_nodes(const void *v1, const void *v2) {
     node_t *n1 = (node_t *)(v1);
     node_t *n2 = (node_t *)(v2);
     if (n1->mean < n2->mean) {
          return -1;
     } else if (n1->mean > n2->mean) {
          return 1;
     } else {
          return 0;
     }
}

static void merge(td_histogram *h) {
     if (h->unmerged_nodes == 0) {
          return;
     }
     int N = h->merged_nodes + h->unmerged_nodes;
     qsort((void *)(h->nodes), N, sizeof(node_t), &compare_nodes);
     double total_count = h->merged_count + h->unmerged_count;
     double denom = 2 * M_PI * total_count * log(total_count);
     double normalizer = h->compression / denom;
     int cur = 0;
     double count_so_far = 0;
     for (int i = 1; i < N; i++) {
          double proposed_count = h->nodes[cur].count + h->nodes[i].count;
          double z = proposed_count * normalizer;
          double q0 = count_so_far / total_count;
          double q2 = (count_so_far + proposed_count) / total_count;
          bool should_add = (z <= (q0 * (1 - q0))) && (z <= (q2 * (1 - q2)));
          if (should_add) {
               h->nodes[cur].count += h->nodes[i].count;
               double delta = h->nodes[i].mean - h->nodes[cur].mean;
               double weighted_delta = (delta * h->nodes[i].count) / h->nodes[cur].count;
               h->nodes[cur].mean += weighted_delta;
          } else {
               count_so_far += h->nodes[cur].count;
               cur++;
               h->nodes[cur] = h->nodes[i];
          }
          if (cur != i) {
               h->nodes[i] = (node_t) {
                    .mean = 0,
                    .count = 0,
               };
          }
     }
     h->merged_nodes = cur+1;
     h->merged_count = total_count;
     h->unmerged_nodes = 0;
     h->unmerged_count = 0;
}
