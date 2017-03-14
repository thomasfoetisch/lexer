
.SUFFIX:

include config.mk

OBJECTS = $(patsubst %.cpp,build/%.o,$(SOURCES))
DEPS = $(patsubst %.cpp,build/%.deps,$(SOURCES))

.PHONY = all deps clean install

all: $(BIN) $(HEADERS) $(LIB)

include $(DEPS)

$(HEADERS): include/lexer/%: src/%
	@echo [INSTALL] $(<:src/%=%)
	@install -m 0644 -D $< $@

$(OBJECTS): build/%.o: %.cpp
	@echo [CXX] $< "depends on " $^
	@mkdir --parents $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DEPS): build/%.deps: %.cpp
	@echo [DEPS] $< "depends on " $^
	@mkdir --parents $(dir $@)
	@$(DEPS_BIN) -std=c++11 -MM -MT build/$*.o $< > $@
	@$(DEPS_BIN) -std=c++11 -MM -MT build/$*.deps $< >> $@

$(BIN): bin/%:
	@echo [LD] $@
	@$(CXX) $(LDFLAGS) -o $@ $^

$(LIB): lib/%:
	@echo [AR] $@
	@$(AR) $(ARFLAGS) $@ $^

deps: $(DEPS)

clean:
	@rm -f $(OBJECTS)
	@rm -f $(DEPS)
	@rm -f $(BIN) $(BIN_TEST)
	@rm -rf build/*
	@rm -rf include/*
	@rm -f $(LIB)

install: $(BIN) $(HEADERS) $(LIB)
	cp $(BIN) $(PREFIX)/$(BIN_DIR)/
	cp -r include/* $(PREFIX)/$(INCLUDE_DIR)/
	cp $(LIB) $(PREFIX)/$(LID_DIR)/
