{
  "targets": [
    {
      "target_name": "vee-eight",
      "sources": [
        "src/addon.cc",
        "src/vee-eight.cc"
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
