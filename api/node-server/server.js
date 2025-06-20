// api/node-server/server.js

const express = require('express');
const { spawn } = require('child_process');
const path    = require('path');

const app = express();
const PORT = process.env.PORT || 8080;

// Path to your C++ CLI binary; adjust if you put it elsewhere
const CLI_PATH = path.resolve(__dirname, '..', '..', 'graph_cli');
const EDGE_FILE = path.resolve(__dirname, '..', '..', 'edges.txt');

// Helper: run graph_cli and stream JSON output
function runCli(mode, args, res) {
  const cp = spawn(CLI_PATH, [mode, EDGE_FILE, ...args]);
  let out = '', err = '';

  cp.stdout.on('data', chunk => out += chunk);
  cp.stderr.on('data', chunk => err += chunk);

  cp.on('close', code => {
    if (code !== 0) {
      console.error(`graph_cli error (${mode}):`, err);
      return res.status(500).json({ error: `CLI exited ${code}` });
    }
    try {
      const json = JSON.parse(out);
      res.json(json);
    } catch (e) {
      console.error('Invalid JSON from CLI:', out);
      res.status(500).json({ error: 'Invalid CLI response' });
    }
  });
}

// GET /neighbors?node=U
app.get('/neighbors', (req, res) => {
  const u = req.query.node;
  if (u === undefined) return res.status(400).json({ error: 'Missing node parameter' });
  runCli('neighbors', [u], res);
});

// GET /bfs?start=U
app.get('/bfs', (req, res) => {
  const u = req.query.start;
  if (u === undefined) return res.status(400).json({ error: 'Missing start parameter' });
  runCli('bfs', [u], res);
});

// GET /shortest?start=U&end=V
app.get('/shortest', (req, res) => {
  const { start, end } = req.query;
  if (start === undefined || end === undefined) {
    return res.status(400).json({ error: 'Missing start or end parameter' });
  }
  runCli('shortest', [start, end], res);
});

// GET /dijkstra?start=U&end=V
app.get('/dijkstra', (req, res) => {
  const { start, end } = req.query;
  if (start === undefined || end === undefined) {
    return res.status(400).json({ error: 'Missing start or end parameter' });
  }
  runCli('dijkstra', [start, end], res);
});

// 404 for anything else
app.use((_, res) => {
  res.status(404).json({ error: 'Not found' });
});

app.listen(PORT, () => {
  console.log(`🚀 Server listening on http://localhost:${PORT}`);
});
