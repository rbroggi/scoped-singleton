### Context - structured logging in transaction info

### Convenience of use 

### Resilient to different flows controls (send log data for early-terminated methods)

### One single network rpc per transaction - network optimization

### Shortcomings 
1. mind your scope: long-living reference
2. mind your scope: no anchor
3. thead-safety needs to be properly studied if you have a read-write API 

# References and acknowledgements

[jsoncons](https://github.com/danielaparker/jsoncons)