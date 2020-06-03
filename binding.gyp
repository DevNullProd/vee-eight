{
  "targets": [
    {
      "target_name": "v-eight",
      "sources": [
        "src/addon.cc",
        "src/v-eight.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
			],
      "defines": [
      ],
      'conditions': [
      ]
    }
  ],
}
