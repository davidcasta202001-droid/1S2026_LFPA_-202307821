#include "ReportGenerator.h"
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <iomanip>
#include <ctime>


ReportGenerator::ReportGenerator(const NodoPtr& arbol,
                                 const std::vector<Token>& tokens,
                                 const ErrorManager& errMgr)
    : arbol(arbol), tokens(tokens), errMgr(errMgr), dotContador(0)
{
    extraerDatos();
}


bool ReportGenerator::generarTodos(const std::string& carpeta) {
    std::string sep = (carpeta.empty() || carpeta.back() == '/') ? "" : "/";
    std::string base = carpeta + sep;
    bool ok = true;
    ok &= generarKanban      (base + "reporte1_kanban.html");
    ok &= generarResponsables(base + "reporte2_responsables.html");
    ok &= generarErrores     (base + "reporte3_errores.html");
    ok &= generarDot         (base + "arbol.dot");
    return ok;
}




static NodoPtr buscarHijo(const NodoPtr& nodo, const std::string& etiqueta) {
    for (auto& h : nodo->hijos)
        if (h->etiqueta == etiqueta) return h;
    return nullptr;
}


static std::vector<NodoPtr> buscarHijos(const NodoPtr& nodo,
                                         const std::string& etiqueta) {
    std::vector<NodoPtr> res;
    for (auto& h : nodo->hijos)
        if (h->etiqueta == etiqueta) res.push_back(h);
    return res;
}

static std::string valorTerminal(const NodoPtr& nodo) {
    if (!nodo) return "";
    if (nodo->esTerminal) return nodo->etiqueta;
    for (auto& h : nodo->hijos)
        if (h->esTerminal) return h->etiqueta;
    return "";
}

static std::string sinComillas(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

void ReportGenerator::extraerTarea(const NodoPtr& nodoTarea, Tarea& t) {
    int cadenas = 0;
    for (auto& h : nodoTarea->hijos) {
        if (h->esTerminal) {
            std::string et = h->etiqueta;
            if (!et.empty() && et.front() == '"') {
                if (cadenas == 0) t.nombre = sinComillas(et);
                cadenas++;
            }
        }
    }

    for (auto& h : nodoTarea->hijos) {
        if (h->etiqueta == "<lista_attrs>") {
            for (auto& attr : h->hijos) {
                if (attr->etiqueta != "<atributo>") continue;
                std::string tipo, valor;
                bool visto = false;
                for (auto& a : attr->hijos) {
                    if (a->esTerminal) {
                        if (!visto) { tipo = a->etiqueta; visto = true; }
                        else if (a->etiqueta != ":") valor = a->etiqueta;
                    } else if (a->etiqueta == "<valor_prioridad>") {
                        for (auto& vp : a->hijos)
                            if (vp->esTerminal) valor = vp->etiqueta;
                    }
                }
                if (tipo == "prioridad")    t.prioridad    = valor;
                if (tipo == "responsable")  t.responsable  = sinComillas(valor);
                if (tipo == "fecha_limite") t.fechaLimite  = valor;
            }
        }
    }
}

void ReportGenerator::extraerColumna(const NodoPtr& nodoColumna, Columna& col) {

    for (auto& h : nodoColumna->hijos) {
        if (h->esTerminal && !h->etiqueta.empty() && h->etiqueta.front() == '"') {
            col.nombre = sinComillas(h->etiqueta);
            break;
        }
    }
    for (auto& h : nodoColumna->hijos) {
        if (h->etiqueta == "<lista_tareas>") {
            for (auto& ht : h->hijos) {
                if (ht->etiqueta == "<tarea>") {
                    Tarea t;
                    extraerTarea(ht, t);
                    col.tareas.push_back(t);
                }
            }
        }
    }
}

void ReportGenerator::extraerDatos() {
    if (!arbol) return;

    for (auto& h : arbol->hijos) {
        if (h->esTerminal && !h->etiqueta.empty() && h->etiqueta.front() == '"') {
            tablero.nombre = sinComillas(h->etiqueta);
            break;
        }
    }

    for (auto& h : arbol->hijos) {
        if (h->etiqueta == "<columnas>") {
            for (auto& hc : h->hijos) {
                if (hc->etiqueta == "<columna>") {
                    Columna col;
                    extraerColumna(hc, col);
                    tablero.columnas.push_back(col);
                }
            }
        }
    }
}

std::string ReportGenerator::htmlHeader(const std::string& titulo,
                                        const std::string& estilosExtra) {
    return R"(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>)" + titulo + R"(</title>
<style>
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: 'Segoe UI', system-ui, sans-serif;
    background: #f0f2f5;
    color: #1a1a2e;
    min-height: 100vh;
  }
  header {
    background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
    color: #fff;
    padding: 24px 40px;
    display: flex;
    align-items: center;
    gap: 16px;
    box-shadow: 0 4px 20px rgba(0,0,0,0.3);
  }
  header .logo {
    font-size: 28px;
    font-weight: 800;
    letter-spacing: -1px;
    color: #4fc3f7;
  }
  header .subtitle { font-size: 13px; color: #90caf9; margin-top: 2px; }
  .nav-pills {
    display: flex; gap: 8px; margin: 0 40px; padding: 16px 0;
  }
  .nav-pills a {
    text-decoration: none; padding: 6px 16px; border-radius: 20px;
    background: #e3f2fd; color: #1565c0; font-size: 13px; font-weight: 500;
    transition: all 0.2s;
  }
  .nav-pills a:hover, .nav-pills a.active {
    background: #1565c0; color: #fff;
  }
  main { padding: 24px 40px 60px; }
  h1 { font-size: 22px; font-weight: 700; margin-bottom: 6px; color: #1a1a2e; }
  .page-desc { color: #546e7a; font-size: 14px; margin-bottom: 24px; }
)" + estilosExtra + R"(
</style>
</head>
<body>
<header>
  <div>
    <div class="logo">TaskScript</div>
    <div class="subtitle">Analizador Léxico y Sintáctico · USAC LFP 2026</div>
  </div>
</header>
<nav class="nav-pills">
  <a href="reporte1_kanban.html">📋 Tablero Kanban</a>
  <a href="reporte2_responsables.html">👥 Carga por Responsable</a>
  <a href="reporte3_errores.html">⚠️ Tabla de Errores</a>
</nav>
<main>
)";
}

std::string ReportGenerator::htmlFooter() {
    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(R"(</main>
<footer style="text-align:center;padding:16px;color:#90a4ae;font-size:12px;
  border-top:1px solid #e0e0e0;background:#fff;margin-top:40px;">
  Generado por TaskScript Analyzer &nbsp;·&nbsp; )") + buf + R"(
</footer>
</body></html>
)";
}

std::string ReportGenerator::colorPrioridad(const std::string& p) {
    if (p == "ALTA")  return "#fff5f5"; // fondo rojo claro
    if (p == "MEDIA") return "#fffde7"; // fondo amarillo claro
    return "#f1f8e9";                   // fondo verde claro (BAJA)
}

std::string ReportGenerator::badgePrioridad(const std::string& p) {
    std::string bg, color;
    if (p == "ALTA")  { bg = "#e53935"; color = "#fff"; }
    else if (p == "MEDIA") { bg = "#f9a825"; color = "#fff"; }
    else               { bg = "#43a047"; color = "#fff"; }
    return "<span style='background:" + bg + ";color:" + color +
           ";padding:2px 10px;border-radius:12px;font-size:11px;"
           "font-weight:700;letter-spacing:0.5px;'>" + p + "</span>";
}

bool ReportGenerator::escribirArchivo(const std::string& ruta,
                                      const std::string& contenido) {
    std::ofstream f(ruta);
    if (!f.is_open()) return false;
    f << contenido;
    return true;
}

bool ReportGenerator::generarKanban(const std::string& ruta) {
    std::string estilos = R"(
  .kanban-header {
    margin-bottom: 24px;
    display: flex; align-items: center; gap: 12px; flex-wrap: wrap;
  }
  .tablero-badge {
    background: #1565c0; color: #fff;
    padding: 4px 14px; border-radius: 20px; font-size: 13px; font-weight: 600;
  }
  .board {
    display: flex; gap: 20px; overflow-x: auto; padding-bottom: 16px;
    align-items: flex-start;
  }
  .column {
    background: #e8edf2;
    border-radius: 12px;
    min-width: 280px;
    max-width: 320px;
    flex-shrink: 0;
    padding: 0 0 12px;
    box-shadow: 0 2px 8px rgba(0,0,0,0.08);
  }
  .column-header {
    background: #1a237e;
    color: #fff;
    padding: 14px 16px;
    border-radius: 12px 12px 0 0;
    font-weight: 700;
    font-size: 14px;
    display: flex; justify-content: space-between; align-items: center;
  }
  .col-count {
    background: rgba(255,255,255,0.25);
    border-radius: 12px; padding: 2px 8px; font-size: 12px;
  }
  .card {
    margin: 10px 10px 0;
    border-radius: 10px;
    padding: 14px;
    box-shadow: 0 1px 4px rgba(0,0,0,0.1);
    border-left: 4px solid transparent;
    transition: transform 0.15s, box-shadow 0.15s;
  }
  .card:hover { transform: translateY(-2px); box-shadow: 0 4px 12px rgba(0,0,0,0.15); }
  .card.alta  { border-left-color: #e53935; background: #fff5f5; }
  .card.media { border-left-color: #f9a825; background: #fffde7; }
  .card.baja  { border-left-color: #43a047; background: #f1f8e9; }
  .card-title { font-weight: 700; font-size: 14px; margin-bottom: 10px; color: #1a1a2e; }
  .card-meta  { font-size: 12px; color: #546e7a; margin-top: 4px; }
  .card-meta span { margin-right: 8px; }
  .empty-col  { color: #90a4ae; font-size: 13px; text-align: center; padding: 20px 10px; }
  .stats-bar  { display:flex; gap:16px; margin-bottom:24px; flex-wrap:wrap; }
  .stat-chip  {
    background:#fff; border-radius:10px; padding:10px 20px;
    box-shadow:0 1px 4px rgba(0,0,0,0.08); text-align:center;
  }
  .stat-chip .num { font-size:24px; font-weight:800; color:#1565c0; }
  .stat-chip .lbl { font-size:11px; color:#90a4ae; }
)";

    std::ostringstream html;
    html << htmlHeader("Reporte 1 — Tablero Kanban | " + tablero.nombre, estilos);

    int totalTareas = 0, totalAlta = 0, totalMedia = 0, totalBaja = 0;
    for (auto& col : tablero.columnas) {
        totalTareas += col.tareas.size();
        for (auto& t : col.tareas) {
            if (t.prioridad == "ALTA")  totalAlta++;
            else if (t.prioridad == "MEDIA") totalMedia++;
            else totalBaja++;
        }
    }

    html << "<h1>📋 Tablero Kanban</h1>\n";
    html << "<p class='page-desc'>Vista visual del tablero <strong>"
         << tablero.nombre << "</strong> con sus columnas y tarjetas de tarea.</p>\n";

    html << "<div class='stats-bar'>\n"
         << "  <div class='stat-chip'><div class='num'>" << tablero.columnas.size()
         << "</div><div class='lbl'>Columnas</div></div>\n"
         << "  <div class='stat-chip'><div class='num'>" << totalTareas
         << "</div><div class='lbl'>Tareas totales</div></div>\n"
         << "  <div class='stat-chip'><div class='num' style='color:#e53935'>"
         << totalAlta << "</div><div class='lbl'>Prioridad ALTA</div></div>\n"
         << "  <div class='stat-chip'><div class='num' style='color:#f9a825'>"
         << totalMedia << "</div><div class='lbl'>Prioridad MEDIA</div></div>\n"
         << "  <div class='stat-chip'><div class='num' style='color:#43a047'>"
         << totalBaja << "</div><div class='lbl'>Prioridad BAJA</div></div>\n"
         << "</div>\n";

    html << "<div class='board'>\n";
    for (auto& col : tablero.columnas) {
        html << "<div class='column'>\n"
             << "  <div class='column-header'>"
             << col.nombre
             << "<span class='col-count'>" << col.tareas.size() << "</span>"
             << "</div>\n";
        if (col.tareas.empty()) {
            html << "<div class='empty-col'>Sin tareas</div>\n";
        }
        for (auto& t : col.tareas) {
            std::string cls = "baja";
            if (t.prioridad == "ALTA")  cls = "alta";
            if (t.prioridad == "MEDIA") cls = "media";
            html << "  <div class='card " << cls << "'>\n"
                 << "    <div class='card-title'>" << t.nombre << "</div>\n"
                 << "    <div>" << badgePrioridad(t.prioridad) << "</div>\n"
                 << "    <div class='card-meta'>"
                 << "<span>👤 " << t.responsable << "</span>"
                 << "<span>📅 " << t.fechaLimite << "</span>"
                 << "</div>\n"
                 << "  </div>\n";
        }
        html << "</div>\n"; 
    }
    html << "</div>\n"; 
    html << htmlFooter();
    return escribirArchivo(ruta, html.str());
}

bool ReportGenerator::generarResponsables(const std::string& ruta) {
    // Acumular datos por responsable
    struct DatosResp {
        int total = 0, alta = 0, media = 0, baja = 0;
    };
    std::map<std::string, DatosResp> mapa;
    int totalGlobal = 0;
    for (auto& col : tablero.columnas) {
        for (auto& t : col.tareas) {
            auto& d = mapa[t.responsable];
            d.total++;
            totalGlobal++;
            if (t.prioridad == "ALTA")       d.alta++;
            else if (t.prioridad == "MEDIA")  d.media++;
            else                              d.baja++;
        }
    }

    std::string estilos = R"(
  table { width:100%; border-collapse:collapse; background:#fff;
    border-radius:12px; overflow:hidden;
    box-shadow:0 2px 8px rgba(0,0,0,0.08); }
  thead { background:#1a237e; color:#fff; }
  th { padding:14px 16px; text-align:left; font-size:13px; font-weight:600; }
  td { padding:14px 16px; font-size:14px; border-bottom:1px solid #f0f0f0; }
  tr:last-child td { border-bottom:none; }
  tr:hover td { background:#f5f9ff; }
  .resp-name { font-weight:700; color:#1a1a2e; display:flex; align-items:center; gap:8px; }
  .avatar {
    width:34px; height:34px; border-radius:50%;
    background:linear-gradient(135deg,#1565c0,#42a5f5);
    color:#fff; font-weight:700; font-size:14px;
    display:inline-flex; align-items:center; justify-content:center;
    flex-shrink:0;
  }
  .bar-wrap { background:#e8edf2; border-radius:8px; height:12px; min-width:120px; overflow:hidden; }
  .bar-fill { height:100%; border-radius:8px;
    background:linear-gradient(90deg,#1565c0,#42a5f5); transition:width 0.3s; }
  .pct-lbl { font-size:12px; color:#546e7a; margin-left:8px; white-space:nowrap; }
  .badge-num {
    display:inline-block; min-width:28px; text-align:center;
    border-radius:6px; padding:2px 8px; font-weight:700; font-size:13px;
  }
  .ba { background:#ffe0e0; color:#c62828; }
  .bm { background:#fff8e1; color:#f57f17; }
  .bb { background:#e8f5e9; color:#2e7d32; }
  .bt { background:#e3f2fd; color:#1565c0; }
)";

    std::ostringstream html;
    html << htmlHeader("Reporte 2 — Carga por Responsable", estilos);
    html << "<h1> Carga por Responsable</h1>\n";
    html << "<p class='page-desc'>Distribución de tareas por persona en el tablero <strong>"
         << tablero.nombre << "</strong>. "
         << "Total de tareas: <strong>" << totalGlobal << "</strong>.</p>\n";

    html << "<table>\n<thead><tr>"
         << "<th>Responsable</th><th>Total</th>"
         << "<th>🔴 ALTA</th><th>🟡 MEDIA</th><th>🟢 BAJA</th>"
         << "<th>Distribución (%)</th>"
         << "</tr></thead>\n<tbody>\n";

    std::vector<std::pair<std::string, DatosResp>> lista(mapa.begin(), mapa.end());
    std::sort(lista.begin(), lista.end(),
              [](auto& a, auto& b){ return a.second.total > b.second.total; });

    for (auto& [nombre, d] : lista) {
        double pct = totalGlobal > 0 ? (100.0 * d.total / totalGlobal) : 0.0;
        char inicial = nombre.empty() ? '?' : (char)std::toupper((unsigned char)nombre[0]);
        html << "<tr>\n"
             << "  <td><div class='resp-name'>"
             << "<span class='avatar'>" << inicial << "</span>"
             << nombre << "</div></td>\n"
             << "  <td><span class='badge-num bt'>" << d.total << "</span></td>\n"
             << "  <td><span class='badge-num ba'>" << d.alta  << "</span></td>\n"
             << "  <td><span class='badge-num bm'>" << d.media << "</span></td>\n"
             << "  <td><span class='badge-num bb'>" << d.baja  << "</span></td>\n"
             << "  <td style='display:flex;align-items:center;'>"
             << "<div class='bar-wrap'><div class='bar-fill' style='width:"
             << std::fixed << std::setprecision(1) << pct << "%'></div></div>"
             << "<span class='pct-lbl'>" << std::fixed << std::setprecision(1)
             << pct << "%</span>"
             << "</td>\n"
             << "</tr>\n";
    }

    if (lista.empty()) {
        html << "<tr><td colspan='6' style='text-align:center;color:#90a4ae;padding:30px'>"
             << "No se encontraron responsables en el tablero.</td></tr>\n";
    }

    html << "</tbody></table>\n";
    html << htmlFooter();
    return escribirArchivo(ruta, html.str());
}

bool ReportGenerator::generarErrores(const std::string& ruta) {
    std::string estilos = R"(
  .summary { display:flex; gap:16px; margin-bottom:24px; flex-wrap:wrap; }
  .chip {
    padding:12px 20px; border-radius:10px; font-weight:700;
    box-shadow:0 1px 4px rgba(0,0,0,0.08);
  }
  .chip-ok  { background:#e8f5e9; color:#2e7d32; }
  .chip-lex { background:#fff3e0; color:#e65100; }
  .chip-syn { background:#fce4ec; color:#c62828; }
  table { width:100%; border-collapse:collapse; background:#fff;
    border-radius:12px; overflow:hidden;
    box-shadow:0 2px 8px rgba(0,0,0,0.08); }
  thead { background:#b71c1c; color:#fff; }
  th { padding:14px 16px; text-align:left; font-size:13px; font-weight:600; }
  td { padding:12px 16px; font-size:13px; border-bottom:1px solid #fce4ec;
    vertical-align:top; }
  tr:last-child td { border-bottom:none; }
  tr:hover td { background:#fff8f8; }
  .badge-lexico    { background:#fff3e0;color:#e65100;padding:2px 10px;
    border-radius:12px;font-size:11px;font-weight:700; }
  .badge-sintactico{ background:#fce4ec;color:#c62828;padding:2px 10px;
    border-radius:12px;font-size:11px;font-weight:700; }
  .lexema { font-family:monospace; background:#f5f5f5;
    padding:2px 6px; border-radius:4px; font-size:12px; }
  .pos { color:#546e7a; font-size:12px; white-space:nowrap; }
  .no-errors { text-align:center; padding:60px; color:#43a047; }
  .no-errors .icon { font-size:48px; margin-bottom:12px; }
  .no-errors p { font-size:16px; font-weight:600; }
)";

    const auto& errores = errMgr.obtenerErrores();
    int nLex = 0, nSin = 0;
    for (auto& e : errores) {
        if (e.tipo == TipoError::LEXICO) nLex++;
        else nSin++;
    }

    std::ostringstream html;
    html << htmlHeader("Reporte 3 — Tabla de Errores", estilos);
    html << "<h1>⚠️ Tabla de Errores</h1>\n";
    html << "<p class='page-desc'>Errores léxicos y sintácticos detectados al analizar el archivo <strong>"
         << tablero.nombre << "</strong>.</p>\n";

    // Chips resumen
    html << "<div class='summary'>\n"
         << "<div class='chip chip-ok'>✅ Total: " << errores.size() << " error(es)</div>\n"
         << "<div class='chip chip-lex'>🟠 Léxicos: " << nLex << "</div>\n"
         << "<div class='chip chip-syn'>🔴 Sintácticos: " << nSin << "</div>\n"
         << "</div>\n";

    if (errores.empty()) {
        html << "<div class='no-errors'>"
             << "<div class='icon'>✅</div>"
             << "<p>¡El archivo no contiene errores!</p>"
             << "</div>\n";
    } else {
        html << "<table>\n<thead><tr>"
             << "<th>No.</th><th>Lexema</th><th>Tipo</th>"
             << "<th>Descripción</th><th>Línea</th><th>Col</th>"
             << "</tr></thead>\n<tbody>\n";

        for (auto& e : errores) {
            std::string badgeClass = (e.tipo == TipoError::LEXICO)
                                   ? "badge-lexico" : "badge-sintactico";
            html << "<tr>\n"
                 << "  <td style='font-weight:700;color:#546e7a'>" << e.numero << "</td>\n"
                 << "  <td><span class='lexema'>" << e.lexema << "</span></td>\n"
                 << "  <td><span class='" << badgeClass << "'>" << e.tipoStr() << "</span></td>\n"
                 << "  <td>" << e.descripcion << "</td>\n"
                 << "  <td class='pos'>" << e.linea << "</td>\n"
                 << "  <td class='pos'>" << e.columna << "</td>\n"
                 << "</tr>\n";
        }
        html << "</tbody></table>\n";
    }

    html << htmlFooter();
    return escribirArchivo(ruta, html.str());
}


void ReportGenerator::dotNodo(std::string& out, const NodoPtr& nodo, int idPadre) {
    if (!nodo) return;

    int idActual = dotContador++;
    std::string etiqueta = nodo->etiqueta;

    std::string etiquetaDot;
    for (char c : etiqueta) {
        if (c == '"') etiquetaDot += "\\\"";
        else if (c == '\\') etiquetaDot += "\\\\";
        else etiquetaDot += c;
    }

    if (nodo->esTerminal) {
        out += "  n" + std::to_string(idActual) +
               " [label=\"" + etiquetaDot + "\", "
               "fillcolor=\"#D6EAF8\", fontcolor=\"#1a237e\", "
               "shape=ellipse];\n";
    } else {
        out += "  n" + std::to_string(idActual) +
               " [label=\"" + etiquetaDot + "\", "
               "fillcolor=\"#1565c0\", fontcolor=\"white\", "
               "shape=box];\n";
    }

    if (idPadre >= 0) {
        out += "  n" + std::to_string(idPadre) +
               " -> n" + std::to_string(idActual) + ";\n";
    }

    for (auto& hijo : nodo->hijos) {
        dotNodo(out, hijo, idActual);
    }
}

bool ReportGenerator::generarDot(const std::string& ruta) {
    dotContador = 0;
    std::string nodos;
    dotNodo(nodos, arbol, -1);

    std::string dot =
        "digraph ArbolDerivacion {\n"
        "  rankdir=TB;\n"
        "  graph [fontname=\"Arial\", bgcolor=\"white\"];\n"
        "  node  [style=filled, fontname=\"Arial\", fontsize=11];\n"
        "  edge  [color=\"#546e7a\", arrowsize=0.7];\n"
        "\n"
        + nodos +
        "}\n";

    return escribirArchivo(ruta, dot);
}
