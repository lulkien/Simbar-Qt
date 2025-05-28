.pragma library

const CatppuccinMocha = {
  rosewater: "#f5e0dc",
  flamingo: "#f2cdcd",
  pink: "#f5c2e7",
  mauve: "#cba6f7",
  red: "#f38ba8",
  maroon: "#eba0ac",
  peach: "#fab387",
  yellow: "#f9e2af",
  green: "#a6e3a1",
  teal: "#94e2d5",
  sky: "#89dceb",
  sapphire: "#74c7ec",
  blue: "#89b4fa",
  lavender: "#b4befe",

  text: "#cdd6f4",
  subtext1: "#bac2de",
  subtext0: "#a6adc8",
  overlay2: "#9399b2",
  overlay1: "#7f849c",
  overlay0: "#6c7086",
  surface2: "#585b70",
  surface1: "#45475a",
  surface0: "#313244",

  base: "#1e1e2e",
  mantle: "#181825",
  crust: "#11111b",
};

var Theme = CatppuccinMocha;

const Color = {
  primary: Theme.blue,
  secondary: Theme.mauve,
  success: Theme.green,
  warning: Theme.yellow,
  danger: Theme.red,
  background: Theme.crust,
  card: Theme.surface0,
  text: Theme.mantle,
  textSecondary: Theme.subtext0,
};

var Font = {
  family: "CodeNewRoman Nerd Font Mono",
};

var Size = {
  barWidth: 3440,
  barHeight: 45,

  defaultBoxSize: 35,
  defaultIconFontSize: 28,

  defaultContentPadding: 8,
  defaultContentFontSize: 16,
};
